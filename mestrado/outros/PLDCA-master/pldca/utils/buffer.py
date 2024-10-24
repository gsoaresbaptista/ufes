import torch
import torch.nn.functional as F

from pldca.data.transform import transform_test


class ReplyBuffer:
    def __init__(self, num_classes, transform=transform_test, imgs_per_class=10):
        super(ReplyBuffer, self).__init__()
        self.transform = transform
        self.m = imgs_per_class
        self.images = [[] for _ in range(num_classes)]
        self.n_cls = num_classes

    def __len__(self):
        return sum([len(data) for data in self.images])

    def get_data(self):
        return self.images

    def remove(self, label):
        self.images[label] = []

    def compute_class_data(self, net, loader, labeler, label_to_id):
        features = [[] for _ in label_to_id.keys()]
        outputs = [[] for _ in label_to_id.keys()]
        images = [[] for _ in label_to_id.keys()]

        # Accumulates all data
        with torch.no_grad():
            for inputs, *_ in loader:
                outs, feas = net(inputs.cuda())
                labels = labeler(feas).tolist()
                outs, feas = outs.cpu(), F.normalize(feas).cpu()
                for idx, label in enumerate(labels):
                    features[label_to_id[label]].append(feas[idx])
                    outputs[label_to_id[label]].append(outs[idx])
                    images[label_to_id[label]].append(inputs[idx])

            # TODO: COMENTAR AQUI
            classes_qtd = 0
            new_label_to_id = {}
            for label, idx in label_to_id.items():
                if len(features[idx]) > 0:
                    new_label_to_id[label] = classes_qtd
                    classes_qtd += 1
            label_to_id = new_label_to_id

            features = [torch.stack(fi) for fi in features if len(fi) > 0]
            outputs = [F.softmax(torch.stack(oi), dim=1) for oi in outputs if len(oi) > 0]
            images = [torch.stack(im) for im in images if len(im) > 0]
            centroids = torch.stack([ci.mean(dim=0) for ci in features])

        return features, outputs, images, centroids, label_to_id

    def construct_exemplar_set(self, net, loader, labeler, threshold_cls):
        label_to_id = {label: i for i, label in enumerate(threshold_cls)}

        net.eval()
        features, outputs, images, centroids, label_to_id = self.compute_class_data(
            net, loader, labeler, label_to_id
        )

        with torch.no_grad():
            for label in label_to_id.keys():
                cur_class_mean = torch.zeros((1, 2048))
                # Clear the buffer in case you are relearning a lesson
                self.images[label].clear()

                for mi in range(self.m):
                    # TODO: REMOVER TRY-EXCEPT
                    try:
                        # Compute distance from centroids
                        x = torch.linalg.norm(
                            centroids[label_to_id[label]]
                            - (cur_class_mean + features[label_to_id[label]]) / (mi + 1),
                            dim=1,
                        )
                    except Exception as error:
                        print("An exception occurred:", error)
                        print(centroids.shape, label_to_id, label)
                        exit(0)

                    index = torch.argmin(x)
                    cur_class_mean += features[label_to_id[label]][index]

                    self.images[label].append(
                        (
                            outputs[label_to_id[label]][index],
                            images[label_to_id[label]][index],
                        )
                    )
