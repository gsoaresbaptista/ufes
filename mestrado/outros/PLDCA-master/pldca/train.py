from datetime import datetime
import torch
import torch.nn.functional as F
from torch.utils.data import DataLoader, WeightedRandomSampler
from pldca.data.dataloader import get_source_dataset
from pldca.net.resnet import resnet50
from pldca.utils.files import load_config
from pldca.utils.parsers import get_experiment_parser
from pldca.data.transform import transform_test
import numpy as np
from sklearn.model_selection import train_test_split

#TODO: Refatorar
def make_weights_for_balanced_classes(dataset, subset_indices=None):
    if subset_indices is not None:
        labels = [dataset.labels[i] for i in subset_indices]
    else:
        labels = dataset.labels
    
    class_counts = np.bincount(labels)
    total_count = len(labels)
    weight_per_class = total_count / (len(class_counts) * class_counts)
    weights = [weight_per_class[label] for label in labels]
    return weights

def val_net(net, test_loader):
    net.eval()
    correct = 0
    total = 0
    gt_list = []
    p_list = []
    for inputs, labels in test_loader:
        inputs = inputs.cuda()
        labels = labels.cuda()
        gt_list.append(labels.cpu().numpy())
        with torch.no_grad():
            outputs, _ = net(inputs)
        output_prob = F.softmax(outputs, dim=1).data
        p_list.append(output_prob[:, 1].detach().cpu().numpy())
        _, predicted = torch.max(outputs, 1)
        total += inputs.size(0)
        num = (predicted == labels).sum()
        correct = correct + num

    acc = 100.0 * correct.item() / total
    return acc


if __name__ == "__main__":
    args = get_experiment_parser()
    config = load_config(args.config)

    # Training settings
    SOURCE = config["shared"]["source"]
    BATCH_SIZE = config["train_settings"]["batch_size"]
    LR = config["train_settings"]["lr"]
    EPOCHS = config["train_settings"]["epochs"]

    # Load dataset settings
    DATASET = config["shared"]["dataset"]
    DATASET_PATH = config["datasets"][DATASET]["path"]
    TOTAL_CLASS_NUM = config["datasets"][DATASET]["total_cls"]
    SOURCE_FILE = config["datasets"][DATASET]["domain_files"][SOURCE]

    # Data augmentation
    from torchvision import transforms
    transform_aug = transforms.Compose(
        [
            transforms.Resize((256, 256)),
            transforms.RandomCrop((224, 224)),
            transforms.RandomHorizontalFlip(),
            transforms.ToTensor(),
            transforms.Normalize((0.485, 0.456, 0.406), (0.229, 0.224, 0.225)),
        ]
    )

    source_dataset_aug = get_source_dataset(DATASET_PATH, SOURCE_FILE, transform_aug)
    source_dataset_test = get_source_dataset(DATASET_PATH, SOURCE_FILE, transform_test)

    train_indices, val_indices = train_test_split(
        list(range(len(source_dataset_aug))), test_size=0.1, stratify=source_dataset_aug.labels, random_state=42
    )
    train_subset = torch.utils.data.Subset(source_dataset_aug, train_indices)
    val_subset = torch.utils.data.Subset(source_dataset_test, val_indices)


    # Train and validation dataloaders
    # weights = make_weights_for_balanced_classes(source_dataset_aug, subset_indices=train_indices)
    # sampler = WeightedRandomSampler(weights, len(weights))
    train_loader = DataLoader(train_subset, BATCH_SIZE, shuffle=True, num_workers=16)
    val_loader = DataLoader(val_subset, BATCH_SIZE, shuffle=False, num_workers=16)


    # Load a resnet pretrained model
    model = resnet50(pretrained=True, num_classes=TOTAL_CLASS_NUM)
    model.cuda()

    param_group = [{"params": v, "lr": LR * (10 if k[:2] == 'fc' else 1)} for k, v in model.named_parameters()]
    optimizer = torch.optim.SGD(param_group, momentum=0.9, weight_decay=5e-4)

    # Early stopping and learning rate scheduler
    early_stop_patience = 10
    early_stop_counter = 0
    best_val_acc = 0.0
    curr_time = datetime.now().strftime("%Y_%m_%d_%H_%M")
    scheduler = torch.optim.lr_scheduler.ReduceLROnPlateau(optimizer, mode='max', patience=3, factor=0.5)

    print(f"| {'Epoch':^5} | {'Train Loss':^10} | {'Val. Acc.':^9} |")

    # Treinamento
    for epoch in range(EPOCHS):
        train_loss, correct, total = 0.0, 0.0, 0.0
        model.train()

        for inputs, targets in train_loader:
            inputs, targets = inputs.cuda(), targets.cuda()
            outputs, features = model(inputs)
            loss = F.cross_entropy(outputs, targets)
            optimizer.zero_grad()
            loss.backward()
            optimizer.step()

            train_loss += loss.item()
            _, predicted = outputs.max(1)
            total += targets.size(0)
            correct += predicted.eq(targets).sum().item()

        train_loss /= len(train_loader)
        val_acc = val_net(model, val_loader)

        print(f"| {epoch+1:^5} | " f"{train_loss:^10.4f} | " f"{val_acc:^9.4f} |")

        scheduler.step(val_acc)

        if val_acc > best_val_acc:
            best_val_acc = val_acc
            torch.save(model.state_dict(), f"weights/{curr_time}_{DATASET}_{SOURCE}.pt")
            early_stop_counter = 0
        else:
            early_stop_counter += 1

        if early_stop_counter >= early_stop_patience:
            print(f"Early stopping at epoch {epoch+1}")
            break

