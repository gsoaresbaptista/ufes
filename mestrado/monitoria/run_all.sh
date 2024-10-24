#!/bin/bash

# Define the base directory
base_dir="all_inputs/modularizacao"

# Iterate over each directory inside the base directory
for dir in "$base_dir"/*; do
    if [ -d "$dir" ]; then
        # Extract the directory name (e.g., 1, 2, etc.)
        dir_name=$(basename "$dir")
        
        # Run the command for each directory
        echo "Running on: $base_dir/$dir_name"
        ./space "$base_dir/$dir_name" < "$base_dir/$dir_name/entrada.txt" > "$base_dir/$dir_name/saida/saida.txt"
    fi
done
