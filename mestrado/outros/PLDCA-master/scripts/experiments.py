import os

import yaml

#TODO: refatorar para usar o Optuna
def make_yaml_test(aug_num_ops_values, aug_magnitude_values, warmup_values, threshold_values):
    base_yaml = {
        "model": {"path": "weights/2024_09_28_21_16_office_home_art.pt"},
        "shared": {"dataset": "office_home", "source": "art", "target": "product"},
        "adapt_settings": {
            "batch_size": 24,
            "lr": 0.001,
            "epochs": 30,
            "increment_cls": 10,
            "reply_buffer": 10,
        },
        "datasets": {
            "office_home": {
                "path": "/home/gbaptista/Documents/dataset/office-home",
                "total_cls": 65,
                "domain_files": {
                    "art": "Art.txt",
                    "clipart": "Clipart.txt",
                    "product": "Product.txt",
                    "real_world": "Real_World.txt",
                },
            }
        },
    }

    for num_ops in aug_num_ops_values:
        for magnitude in aug_magnitude_values:
            for warmup in warmup_values:
                for threshold in threshold_values:
                    for dynamic_threshold in [True, False]:
                        base_yaml["shared"]["aug_num_ops"] = num_ops
                        base_yaml["shared"]["aug_magnitude"] = magnitude
                        base_yaml["adapt_settings"]["warm_up_epoch"] = warmup
                        base_yaml["adapt_settings"]["threshold"] = threshold
                        base_yaml["adapt_settings"]["dynamic_threshold"] = dynamic_threshold
                        filename = f"config_aug_ops_{num_ops}_magnitude_{magnitude}_warmpup_{warmup}_threshold_{threshold}_{dynamic_threshold}.yaml"
                        with open(filename, "w") as file:
                            yaml.dump(base_yaml, file)
                        os.system(
                            f"python pldca/adapt.py {filename} > runs/{os.path.basename(filename)}.txt"
                        )
                        os.remove(filename)

aug_num_ops_values = [2, 3, 1, 4, 5, 6]
aug_magnitude_values = [20, 1, 2, 13, 7, 9, 11, 15, 16, 3, 5]
warmup_values = [3, 2, 4, 1, 5]
threshold_values = [0.1]
make_yaml_test(aug_num_ops_values, aug_magnitude_values, warmup_values, threshold_values)
