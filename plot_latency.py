import pandas as pd
import matplotlib.pyplot as plt
import argparse
import numpy as np

# Argument parser for the CSV file path
parser = argparse.ArgumentParser(description="Plot bandwidth data from CSV file")
parser.add_argument("csv_file", type=str, help="Path to the CSV file")
args = parser.parse_args()

# Read the CSV file into a pandas DataFrame
data = pd.read_csv(args.csv_file)

# Get unique thread counts from the 'threads' column
thread_counts = data['threads'].unique()

# Create a single plot for all thread counts
plt.figure(figsize=(10, 6))

# Define a list of colors for each thread count
colors = ['b', 'g', 'r', 'c', 'm', 'y', 'k']

for idx, thread_count in enumerate(thread_counts):
    thread_data = data[data['threads'] == thread_count]
    x_values = thread_data['memsize']
    y_values = thread_data['bandwidth']
    
    color = colors[idx % len(colors)]
    
    plt.plot(x_values, y_values, color=color, linestyle='-', linewidth=2, label=f'Threads {thread_count}')

plt.xlabel('Memsize (KB)')
plt.ylabel('Bandwidth (GB/s)')
plt.title('Bandwidth vs Memsize')
plt.legend()
plt.grid()

# Convert memsize to log2 scale
plt.xscale('log', base=2)
x_ticks = [2 ** i for i in range(int(np.log2(min(x_values))), int(np.log2(max(x_values))) + 1)]
plt.xticks(x_ticks, labels=x_ticks, rotation=45)

# Set y-axis ticks to multiples of 2
plt.yscale('log', base=2)
y_ticks = [2 ** i for i in range(int(np.log2(min(y_values))), int(np.log2(max(y_values))) + 1)]
plt.yticks(y_ticks, labels=y_ticks)

# Save the plot as vectorized PNG and PDF
output_filename = "bandwidth_plot_linear_y_axis"

# Save as PNG
png_filename = f"{output_filename}.png"
plt.savefig(png_filename, format='png', dpi=300, bbox_inches='tight')

# Save as PDF
pdf_filename = f"{output_filename}.pdf"
plt.savefig(pdf_filename, format='pdf', bbox_inches='tight')

# Show the plot
plt.show()

print(f"Plot saved as {png_filename} and {pdf_filename}")
