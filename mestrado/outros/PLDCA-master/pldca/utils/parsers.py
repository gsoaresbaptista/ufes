import argparse

# TODO: Refatorar e juntar com o files.py


def default_parser():
    parser = argparse.ArgumentParser(description="Experiment Configuration")
    return parser


def experiment_parser(parser):
    parser.add_argument("config", type=str, help="Path to the YAML configuration file.")
    return parser


def get_experiment_parser():
    return experiment_parser(default_parser()).parse_args()
