import torch
import torch.nn.functional as F


class OnlineLabeler(torch.nn.Module):
    def __init__(self, ck, current_classes) -> None:
        super(OnlineLabeler, self).__init__()
        self.current_classes = current_classes
        self.ck_norm = F.normalize(ck, p=2, dim=1)[current_classes].T
        self.indice_to_class = {i: cls for i, cls in enumerate(current_classes)}

    def forward(self, features):
        f_norm = F.normalize(features, p=2, dim=1)
        preds = torch.matmul(f_norm, self.ck_norm).argmax(dim=1)
        preds = torch.tensor([self.indice_to_class[prob.item()] for prob in preds])
        return preds.cuda()


def get_confidence_classes(net, loader, cls_num, alpha=0.1):
    net.eval()
    pred_bank = torch.zeros(cls_num).cuda()

    # Loop through data
    with torch.no_grad():
        for img_data, *_ in loader:
            output, _ = net(img_data.cuda())
            pred_bank += F.softmax(output, dim=1).sum(dim=0)
        # min max scaler
        min_prob, max_prob = pred_bank.min(), pred_bank.max()
        pred_bank = (pred_bank - min_prob) / (max_prob - min_prob)

    confident_dict = {
        idx: round(value.item(), 4)
        for idx, value in enumerate(pred_bank)
        if value >= alpha  # Check the threshold
    }
    rejecteds_dict = {
        idx: round(value.item(), 4)
        for idx, value in enumerate(pred_bank)
        if idx not in confident_dict
    }

    return confident_dict, rejecteds_dict


def adjust_centroids(net, loader, labeler):
    sum_weighted_features = torch.zeros((65, 2048)).cuda()
    normalization_factors = torch.zeros((65, 1)).cuda()
    with torch.no_grad():
        for inputs, *_ in loader:
            inputs = inputs.cuda()
            _, features = net(inputs)
            preds = labeler(features)
            for idx, cls in enumerate(preds.tolist()):
                sum_weighted_features[cls] += features[idx]
                normalization_factors[cls] += 1
    centroids = sum_weighted_features / normalization_factors
    return centroids


def obtain_centroids(net, loader, cls_num):
    net.eval()
    sum_weighted_features = torch.zeros((2048, cls_num)).cuda()
    normalization_factors = torch.zeros(cls_num).cuda()

    with torch.no_grad():
        for inputs, *_ in loader:
            inputs = inputs.cuda()
            odd_i, feature_i = net(inputs)
            odd_i = F.softmax(odd_i, dim=1)
            weighted_features = torch.einsum("ij,ik->ijk", feature_i, odd_i)
            sum_weighted_features += weighted_features.sum(dim=0)
            normalization_factors += odd_i.sum(dim=0)

    centroids = sum_weighted_features / normalization_factors.unsqueeze(0)
    centroids = centroids.T

    return centroids
