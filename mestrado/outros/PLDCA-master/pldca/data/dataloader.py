import torch
from torchvision import transforms

from pldca.data.dataset import FileListDataset
from pldca.data.transform import transform_test


# TODO: REFATORAR OS CÓDIGOS DESSE MÓDULO
def get_source_dataset(dataset_path, source_file, transform=transform_test):
    return FileListDataset(
        list_path=source_file,
        path_prefix=dataset_path,
        transform=transform,
    )


def get_target_dataset(dataset_path, target_file, transform=transform_test, target_classes=None):
    if target_classes is not None:
        filter = lambda x: x in target_classes
    return FileListDataset(
        list_path=target_file,
        path_prefix=dataset_path,
        transform=transform,
        filter=filter,
    )


def get_source_dataloaders(dataset_path, source_file, batch_size):
    s_total_ds = get_source_dataset(dataset_path, source_file)
    s_total_loader = torch.utils.data.DataLoader(
        s_total_ds,
        batch_size=batch_size,
        shuffle=True,
        num_workers=8,
    )
    return s_total_ds, s_total_loader


def get_target_dataloaders(icrement_id, dataset_path, target_file, batch_size, aug_n, aug_m):
    target_train_classes = list(range(10 * icrement_id, 10 * icrement_id + 10))
    target_test_classes = list(range(0, 10 * icrement_id + 10))

    t_train_ds = FileListDataset(
        list_path=target_file,
        path_prefix=dataset_path,
        transform=transform_test,
        augment_transform=transforms.RandAugment(aug_n, aug_m),
        filter=(lambda x: x in target_train_classes),
    )
    t_train_loader = torch.utils.data.DataLoader(
        dataset=t_train_ds,
        batch_size=batch_size,
        shuffle=True,
        num_workers=8,
    )

    t_test_ds = FileListDataset(
        list_path=target_file,
        path_prefix=dataset_path,
        transform=transform_test,
        filter=(lambda x: x in target_test_classes),
    )
    t_test_loader = torch.utils.data.DataLoader(
        t_test_ds,
        batch_size=batch_size,
        shuffle=False,
        num_workers=8,
    )

    return (t_train_ds, t_test_ds), (t_train_loader, t_test_loader)

def get_target_total_dataloaders(icrement_id, dataset_path, target_file, batch_size,):
    target_train_classes = list(range(10 * icrement_id, 10 * icrement_id + 10))

    t_train_ds = FileListDataset(
        list_path=target_file,
        path_prefix=dataset_path,
        transform=transform_test,
        filter=(lambda x: x in target_train_classes),
    )
    t_train_loader = torch.utils.data.DataLoader(
        dataset=t_train_ds,
        batch_size=batch_size,
        shuffle=True,
        num_workers=8,
    )

    return t_train_loader
