import numpy as np
import torch
import torch.nn.functional as F


# TODO: Refatorar a função
def val_pclass(net, dataset_test, n_share, unk_class=None):
    if unk_class is None:
        unk_class = n_share
    net.eval()

    class_list = [i for i in range(n_share)]
    class_list.append(unk_class)
    per_class_num = np.zeros((n_share + 1))
    per_class_correct = np.zeros((n_share + 1)).astype(np.float32)

    with torch.no_grad():
        for img_t, label_t in dataset_test:
            out_t, _ = net(img_t.cuda())
            _, out_t = F.softmax(out_t, dim=1).max(1)
            pred = out_t.cpu().numpy()

            for i, t in enumerate(class_list):
                t_ind = np.where(label_t.numpy() == t)
                correct_ind = np.where(pred[t_ind[0]] == t)
                per_class_correct[i] += float(len(correct_ind[0]))
                per_class_num[i] += float(len(t_ind[0]))

    per_class_acc = np.round(per_class_correct / (per_class_num + 1e-8), 3)

    return per_class_acc
