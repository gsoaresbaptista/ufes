from datetime import datetime
from statistics import mean
import copy

import numpy as np
import torch
import torch.nn.functional as F

from pldca.data.dataloader import get_source_dataloaders, get_target_dataloaders, get_target_total_dataloaders
from pldca.data.dataset import ReplyDataset
from pldca.net.resnet import resnet50
from pldca.utils.buffer import ReplyBuffer
from pldca.utils.files import load_config
from pldca.utils.losses import loss_inter_domain_contrastive
from pldca.utils.parsers import get_experiment_parser
from pldca.utils.prototype import OnlineLabeler, get_confidence_classes, obtain_centroids
from pldca.utils.test import val_pclass

if __name__ == "__main__":
    args = get_experiment_parser()
    config = load_config(args.config)

    # Training settings
    SOURCE = config["shared"]["source"]
    TARGET = config["shared"]["target"]
    BATCH_SIZE = config["adapt_settings"]["batch_size"]
    LR = config["adapt_settings"]["lr"]
    EPOCHS = config["adapt_settings"]["epochs"]
    AUG_N = config["shared"]["aug_num_ops"]
    AUG_M = config["shared"]["aug_magnitude"]
    WARM_UP_EPOCH = config["adapt_settings"]["warm_up_epoch"]
    THRESHOLD = config["adapt_settings"]["threshold"]
    DYNAMIC_ALPHA = config["adapt_settings"]["dynamic_threshold"]

    # Load dataset settings
    DATASET = config["shared"]["dataset"]
    DATASET_PATH = config["datasets"][DATASET]["path"]
    TOTAL_CLASS_NUM = config["datasets"][DATASET]["total_cls"]
    SOURCE_FILE = config["datasets"][DATASET]["domain_files"][SOURCE]
    TARGET_FILE = config["datasets"][DATASET]["domain_files"][TARGET]

    # Load pretrained model
    pretrained_net = resnet50(pretrained=False, num_classes=TOTAL_CLASS_NUM).cuda()
    pretrained_net.load_state_dict(torch.load(config["model"]["path"], weights_only=True))
    pretrained_net.eval()
    net = resnet50(pretrained=True, num_classes=TOTAL_CLASS_NUM)
    net = net.cuda()

    # Utilities
    mu_history = {i: 0 for i in range(TOTAL_CLASS_NUM)}
    optimizer = torch.optim.SGD(net.parameters(), lr=LR, momentum=0.9, weight_decay=5e-4)
    s_ds, s_loader = get_source_dataloaders(DATASET_PATH, SOURCE_FILE, BATCH_SIZE)
    dck = torch.zeros(TOTAL_CLASS_NUM, 2048).cuda()
    tck = torch.zeros(TOTAL_CLASS_NUM, 2048).cuda()
    reply_buffer = ReplyBuffer(TOTAL_CLASS_NUM)
    tck_classes, dck_classes = set(), set()

    # Experiment name settigs
    now = datetime.now()
    experiment_prefix = now.strftime("%d-%m-%Y_%H-%M")

    for incre_id in range(TOTAL_CLASS_NUM // 10):
        # Dataset and Dataloaders
        data = get_target_dataloaders(incre_id, DATASET_PATH, TARGET_FILE, BATCH_SIZE, AUG_N, AUG_M)
        (t_train_ds, t_test_ds), (t_train_loader, t_test_loader) = data
        t_total_loader = get_target_total_dataloaders(incre_id, DATASET_PATH, TARGET_FILE, BATCH_SIZE)

        # Class-Level Label Distillation
        target_cls, target_rejects = get_confidence_classes(pretrained_net, t_total_loader, TOTAL_CLASS_NUM, THRESHOLD)
        labels_to_remove = []

        for label, mu_til in target_cls.items():
            if mu_til > mu_history[label]:
                if label in tck_classes:
                    tck_classes.remove(label)
                    reply_buffer.remove(label)
                mu_history[label] = mu_til
            else:
                labels_to_remove.append(label)

        for label in labels_to_remove:
            del target_cls[label]

        # Target Label Detection
        ck = obtain_centroids(pretrained_net, t_total_loader, TOTAL_CLASS_NUM)
        labeler = OnlineLabeler(ck, list(target_cls.keys()))

        if incre_id > 0:
            reply_dataset = ReplyDataset(reply_buffer.get_data())
            reply_loader = torch.utils.data.DataLoader(
                reply_dataset, batch_size=BATCH_SIZE, shuffle=True
            )
            reply_iterator = iter(reply_loader)

        # Log table header
        target_cls_str = str(list(target_cls.keys()))
        table_header = f"Increment {incre_id+1} | Classes {target_cls_str}"
        half = int((121 - len(table_header)) / 2)
        table_header = f"{' ' * half}{table_header}{' ' * (half)}"
        table_header = f"|{table_header}|" if len(table_header) >= 121 else f"|{table_header} |"

        print(
            f"{table_header}\n"
            f"| {'Epoch':^5} | {'CE Source':^9} | {'CE Target':^9} | {'L_class':^9} "
            f"| {'L_IDC':^9} | {'L_IC':^9} | {'Distill':^9} "
            f"| {'L_total':^9} | {'Accuracy':^9} | {'Removed Classes':^15} |"
        )

        # Iterate through each epoch for one increment
        for epoch in range(EPOCHS):
            min_iterations = min(len(t_train_loader), len(s_loader))
            source_loader = iter(s_loader)
            target_train_loader = iter(t_train_loader)
            target_test_loader = iter(t_test_loader)

            log_dict = {
                "loss_ce_source": [],
                "loss_ce_target": [],
                "loss_lclass": [],
                "loss_idc": [],
                "loss_ic": [],
                "loss_total": [],
                "acc": [],
                "loss_distill": [],
            }
            removed_cls = []

            for iter_idx in range(min_iterations):
                optimizer.zero_grad()
                net.train()

                # Makes all losses start as 0, to avoid problems when no class is detected
                ce_source, ce_target, lclass, l_idc, l_ic, distill_loss = [
                    torch.zeros([1]).cuda() for _ in range(6)
                ]

                # Source data cross-entropy optimization
                source_inputs, source_labels = next(source_loader)
                source_inputs = source_inputs.cuda()
                source_labels = source_labels.cuda()
                source_ouputs, source_feas = net(source_inputs)
                ce_source = F.cross_entropy(source_ouputs, source_labels)

                # Target data optimization
                target_inputs, *_, target_augmented = next(target_train_loader)
                target_inputs = target_inputs.cuda()
                target_ouputs, target_feas = net(target_inputs)
                pseudo_labels = labeler(target_feas)

                # TODO: COMENTAR
                with torch.no_grad():
                    if epoch <= WARM_UP_EPOCH:
                        confidence_probs, _ = pretrained_net(target_inputs)
                    else:
                        confidence_probs = target_ouputs

                # Get confident and confuse classes
                preds = F.softmax(confidence_probs, dim=1).max(dim=1)
                confident_idx = torch.nonzero(preds.values >= 0.6).squeeze().cpu()
                confuse_idx = torch.nonzero(preds.values < 0.6).squeeze().cpu()

                # Convert output with just one element to list again
                if confident_idx.dim() == 0:
                    confident_idx = confident_idx.unsqueeze(0)

                if confuse_idx.dim() == 0:
                    confuse_idx = confuse_idx.unsqueeze(0)
                confuse_idx = confuse_idx.tolist()

                if len(confident_idx):
                    ce_target = F.cross_entropy(
                        target_ouputs[confident_idx], pseudo_labels[confident_idx]
                    )

                pseudo_labels = pseudo_labels.cpu()
                classes = pseudo_labels.unique()

                with torch.no_grad():
                    # Update source centers
                    for label in source_labels.unique().tolist():
                        (indices,) = torch.where(source_labels == label)
                        nc = source_feas[indices, :].mean(dim=0)
                        dck[label] = 0.99 * dck[label] + 0.01 * nc if label in dck_classes else nc
                        dck_classes.add(label)

                    # Update target centers
                    for label in classes.tolist():
                        (indices,) = torch.where(pseudo_labels == label)
                        indices = np.intersect1d(indices, confident_idx)
                        if len(indices):
                            nc = target_feas[indices, :].mean(dim=0)
                            tck[label] = (
                                0.99 * tck[label] + 0.01 * nc if label in tck_classes else nc
                            )
                            tck_classes.add(label)

                if epoch > WARM_UP_EPOCH:
                    # Compute Loss IDC (confident contrastive loss)
                    if len(tck_classes) > 0:
                        denominator = len(tck_classes)

                        for cls_label in tck_classes:
                            # Ignore centroids that do not have a positive pair
                            if cls_label not in dck_classes:
                                denominator -= 1
                                continue

                            centroid_k = tck[cls_label].unsqueeze(0)
                            positive = dck[cls_label].reshape(-1, 2048)
                            negatives = torch.concat(
                                [
                                    dck[[label for label in dck_classes if label != cls_label]],
                                    tck[[label for label in tck_classes if label != cls_label]],
                                ]
                            )

                            l_idc += loss_inter_domain_contrastive(positive, centroid_k, negatives)
                        l_idc /= denominator

                    # Compute Loss IC (instance-level loss)
                    if len(confuse_idx) > 0:
                        # Create augmented images for confuse classes
                        target_augmented = target_augmented.cuda()
                        _, augmented_images = net(target_augmented)
                        for idx in confuse_idx:
                            zu = target_feas[idx].unsqueeze(0)
                            positive = augmented_images[idx].unsqueeze(0)
                            negatives = augmented_images[[i for i in confuse_idx if i != idx]]
                            l_ic += loss_inter_domain_contrastive(zu, positive, negatives)
                        l_ic /= len(confuse_idx)

                # Distillation Loss
                if incre_id > 0:
                    data_buffer = next(reply_iterator, None)

                    # Update the buffer if it has reached the end
                    if data_buffer is None:
                        reply_iterator = iter(reply_loader)
                        data_buffer = next(reply_iterator)

                    reply_images, reply_old_preds = data_buffer
                    reply_images = reply_images.cuda()
                    reply_old_preds = reply_old_preds.cuda()

                    # Distillation
                    reply_ouputs, _ = net(reply_images)
                    reply_ouputs = torch.log(F.softmax(reply_ouputs, dim=1))
                    soft_pred_loss = torch.sum(-1 * reply_old_preds * reply_ouputs, dim=1)
                    distill_loss += torch.mean(soft_pred_loss)

                # Compute total loss
                lclass = ce_source + ce_target
                total_loss = lclass + l_idc + l_ic + distill_loss

                # Save to log dictionary
                log_dict["loss_ce_source"].append(ce_source.item())
                log_dict["loss_ce_target"].append(ce_target.item())
                log_dict["loss_lclass"].append(lclass.item())
                log_dict["loss_total"].append(total_loss.item())
                log_dict["loss_idc"].append(l_idc.item())
                log_dict["loss_ic"].append(l_ic.item())
                log_dict["loss_distill"].append(distill_loss.item())

                total_loss.backward()
                optimizer.step()

            # Compute Accuracy
            acc_list = val_pclass(net, target_test_loader, TOTAL_CLASS_NUM)
            if incre_id < TOTAL_CLASS_NUM // 10:
                acc_list = acc_list[: (incre_id + 1) * 10]
            val_acc = mean(acc_list)
            log_dict["acc"].append(val_acc)

            if epoch >= WARM_UP_EPOCH:
                # Filter classes with low confidence with the target model
                alpha = (0.1 + 0.2 * (epoch - WARM_UP_EPOCH)/(EPOCHS - WARM_UP_EPOCH)) if DYNAMIC_ALPHA else 0.1
                conf_cls, rejects = get_confidence_classes(net, t_total_loader, TOTAL_CLASS_NUM, alpha)
                removed_cls = [i for i in target_cls if i in rejects]
                target_cls = {key: conf_cls[key] for key in target_cls if key in conf_cls}
                tck_classes = {label for label in tck_classes if label in target_cls}

                # Update target centroids
                labeler = OnlineLabeler(tck, list(target_cls.keys()))

            # Build removed classes string
            removed_cls_str = ", ".join(map(str, removed_cls)) if len(removed_cls) else "--"

            print(
                f"| {epoch+1:^5} | "
                f"{mean(log_dict['loss_ce_source']):^9.4f} | "
                f"{mean(log_dict['loss_ce_target']):^9.4f} | "
                f"{mean(log_dict['loss_lclass']):^9.4f} | "
                f"{mean(log_dict['loss_idc']):^9.4f} | "
                f"{mean(log_dict['loss_ic']):^9.4f} | "
                f"{mean(log_dict['loss_distill']):^9.4f} | "
                f"{mean(log_dict['loss_total']):^9.4f} | "
                f"{log_dict['acc'][-1]:^9.4f} | "
                f"{removed_cls_str:^15} |"
            )

        # Prototype buffer construction
        reply_buffer.construct_exemplar_set(net, t_total_loader, labeler, list(target_cls.keys()))
