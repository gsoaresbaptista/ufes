import os
import argparse

def test_file(folder, path):
    posible_inputs = ['a', 's', 'd', ' ', 'j', 'l']
    with open(path, 'r') as txt:
        lines = txt.readlines()
        for line_number, line in enumerate(lines):
            if not (
                (len(line) == 2 and line[0] in posible_inputs and line[1] == '\n') or
                (len(lines) == 1 and line[0] == '\n')
            ):
                print(f'Entrada {folder}, Linha {line_number + 1} apresenta erro!')
                return line
    return None

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('path', type=str)
    args = parser.parse_args()

    for folder in os.listdir(args.path):
        test_file(folder, os.path.join(args.path, folder, 'entrada.txt'))
