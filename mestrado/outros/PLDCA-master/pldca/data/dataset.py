from collections import Counter
from os.path import join

import torch
from PIL import Image
from torch.utils.data import Dataset


# TODO: REFATORAR OS CÓDIGOS DESSE MÓDULO
def join_path(*a):
    return join(*a)


class BaseImageDataset(Dataset):
    def __init__(self, transform=None, augment_transform=None):
        self.transform = transform or (lambda x: x)
        self.augment_transform = augment_transform
        self.datas = []
        self.labels = []

    def __getitem__(self, index):
        return_data = []
        im = Image.open(self.datas[index]).convert("RGB")

        if self.augment_transform is not None:
            aug_im = self.transform(self.augment_transform(im))
        im = self.transform(im)

        return_data.append(im)
        return_data.append(self.labels[index])

        if self.augment_transform is not None:
            return_data.append(aug_im)

        return return_data

    def __len__(self):
        return len(self.datas)


class FileListDataset(BaseImageDataset):
    def __init__(
        self,
        list_path,
        path_prefix="",
        transform=None,
        augment_transform=None,
        filter=None,
    ):
        super(FileListDataset, self).__init__(
            transform=transform, augment_transform=augment_transform
        )
        self.list_path = list_path
        self.path_prefix = path_prefix
        self.filter = filter or (lambda x: True)

        with open(join(self.path_prefix, self.list_path), "r") as f:
            data = [
                [
                    line.split()[0],
                    line.split()[1] if len(line.split()) > 1 else "0",
                ]
                for line in f.readlines()
                if line.strip()
            ]
            self.datas = [join_path(self.path_prefix, x[0]) for x in data]
            self.labels = [int(x[1]) for x in data]

        if filter is not None:
            ans = [(x, y) for (x, y) in zip(self.datas, self.labels) if self.filter(y)]
            self.datas, self.labels = zip(*ans)

        self.num_classes = len(Counter(self.labels))

    def clone(self):
        return FileListDataset(
            list_path=self.list_path,
            path_prefix=self.path_prefix,
            transform=self.transform,
            num_classes=self.num_classes,
            filter=self.filter,
        )


class ReplyDataset(Dataset):
    def __init__(self, buffer_data):
        super(Dataset, self).__init__()
        images, preds = [], []

        for cls_buffer in buffer_data:
            for pred, im in cls_buffer:
                preds.append(pred)
                images.append(im)

        self.images = torch.stack(images)
        self.soft_pred = torch.stack(preds)

    def __getitem__(self, index):
        return self.images[index], self.soft_pred[index]

    def __len__(self):
        return self.images.shape[0]


class AugmentedDataset(torch.utils.data.Dataset):
    def __init__(self, dataset, transform, aug_transform, augment_times=3):
        self.dataset = dataset
        self.augment_times = augment_times
        self.transform = transform
        self.aug_transform = aug_transform

    def __len__(self):
        return len(self.dataset) * self.augment_times

    def __getitem__(self, idx):
        original_idx = idx % len(self.dataset)
        data, label = self.dataset[original_idx]

        if idx // len(self.dataset) > 0:
            data = self.aug_transform(data)
        data = self.transform(data)

        return data, label


class TransformDataset(torch.utils.data.Dataset):
    def __init__(self, dataset, transform=None):
        self.dataset = dataset
        self.transform = transform

    def __len__(self):
        return len(self.dataset)

    def __getitem__(self, idx):
        image, label = self.dataset[idx]

        if self.transform:
            image = self.transform(image)

        return image, label
