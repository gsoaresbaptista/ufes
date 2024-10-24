import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

# Load the CSV file
file_path = 'algorithm_comparison.csv'  # Replace with your file path
data = pd.read_csv(file_path)

# Clean up column names by stripping leading/trailing whitespace
data.columns = data.columns.str.strip()

# Apply a moving average to the measured times (for smoother curves)
window_size = 5  # You can adjust the window size for the moving average
data['Hierholzer Time (s)'] = data['Hierholzer Time (s)'].rolling(window=window_size).mean()
data['Fleury Time (s)'] = data['Fleury Time (s)'].rolling(window=window_size).mean()

# Set the seaborn style
sns.set(style="whitegrid")

# Define a range of edge sizes
edge_sizes = np.linspace(data['Edge Size'].min(), data['Edge Size'].max(), 100)

# Theoretical complexities
hierholzer_theoretical = edge_sizes
fleury_theoretical = (edge_sizes ** 2) / edge_sizes.max()

# Create the plot
plt.figure(figsize=(10, 6))

# Plot measured times
sns.lineplot(x='Edge Size', y='Hierholzer Time (s)', data=data, marker='o', label="Hierholzer (medido)")
sns.lineplot(x='Edge Size', y='Fleury Time (s)', data=data, marker='o', label="Fleury (medido)")

# Set labels in Portuguese
plt.xlabel('Tamanho do Grafo (Número de Arestas)', fontsize=14)
plt.ylabel('Tempo de Execução (segundos)', fontsize=14)

# Remove the main title
plt.legend(title="Algoritmo", fontsize=12)

# Save the plot as an EPS file
plt.savefig('algorithm_comparison_with_complexity_edges.eps', format='eps')
