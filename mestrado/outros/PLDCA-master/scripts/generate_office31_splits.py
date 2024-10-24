import os
#TODO: Refatorar
dataset_dir = "/home/gabriel/Documents/dataset/office-31"
domains = ["Amazon", "Webcam", "DSLR"]


def get_label_mapping(domain_path):
    categories = sorted(os.listdir(domain_path))
    label_mapping = {category: idx for idx, category in enumerate(categories)}
    return label_mapping


def generate_txt_file(domain):
    domain_path = os.path.join(dataset_dir, domain)
    label_mapping = get_label_mapping(domain_path)
    output_file = f"{domain}.txt"

    with open(output_file, "w") as f:
        for category, label in label_mapping.items():
            category_path = os.path.join(domain_path, category)
            if os.path.isdir(category_path):
                for img_file in sorted(os.listdir(category_path)):
                    if img_file.endswith((".jpg", ".png", ".jpeg")):
                        img_relative_path = os.path.join(domain, category, img_file)
                        f.write(f"{img_relative_path} {label}\n")

    print(f"{output_file} has been generated.")


for domain in domains:
    generate_txt_file(domain)

print("All txt files for Office-31 have been generated.")
