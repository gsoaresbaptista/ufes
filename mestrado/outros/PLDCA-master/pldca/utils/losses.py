import torch
import torch.nn.functional as F

# TODO: Refatorar
def loss_inter_domain_contrastive(cdk, ctk, cqr):
    numerator = torch.exp((F.cosine_similarity(cdk, ctk) - 0.02) / 5)
    denominator = numerator + torch.exp((F.cosine_similarity(cqr, ctk)) / 5).sum(dim=0)
    return -torch.log(numerator / denominator).mean()
