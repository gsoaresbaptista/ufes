import yaml


# TODO: Refatorar e juntar com o parsers.py
def load_config(config_path):
    with open(config_path, "r") as file:
        config = yaml.safe_load(file)
    return config
