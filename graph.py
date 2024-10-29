import sys
import ast
import matplotlib.pyplot as plt
import pandas as pd

def plot_execution(all_values, title):
    plt.figure(figsize=(10, 6))
    sizes = [500, 1000, 2000, 4000, 8000, 16000, 32000, 64000, 80000, 100000]
    labels = ['Bubble Sort', 'Insertion Sort', 'Merge Sort', 'Quick Sort', 'Heap', 'Radix']

    if len(all_values) == 5 :
        labels = labels[1:]
    elif len(all_values) == 4:
        labels = labels[2:]

    for i, execution_times in enumerate(all_values):
        plt.plot(sizes, execution_times, marker='o', linestyle='-', label=labels[i] if i < len(labels) else f'Algorithm {i+1}')
    
    
    plt.title(f'Execution Time vs. Input Size (${title}$)', fontsize=16)
    plt.xlabel('Input Size ($n$)', fontsize=12)
    plt.ylabel('Execution Time (ms)', fontsize=12)
    plt.legend()
    plt.grid(True)
    #plt.savefig(f'Execution_time VS Size plot_{title}.png', dpi=300)
    print(f"Plot saved as 'Execution_time VS Size plot_{title}.png'")
    plt.show()

def compute_pearson_correlation(x, y):
    n = len(x)
    sum_x = sum(x)
    sum_y = sum(y)
    sum_x_sq = sum(xi**2 for xi in x)
    sum_y_sq = sum(yi**2 for yi in y)
    sum_xy = sum(xi * yi for xi, yi in zip(x, y))

    numerator = n * sum_xy - sum_x * sum_y
    denominator = ((n * sum_x_sq - sum_x**2) * (n * sum_y_sq - sum_y**2)) ** 0.5
    if denominator == 0:
        return 0
    return numerator / denominator

def compute_correlations(all_values):
    sizes = [500, 1000, 2000, 4000, 8000, 16000, 32000, 64000, 80000, 100000]
    correlations = []

    for execution_times in all_values:
        if len(execution_times) != len(sizes):
            raise ValueError("Each list of execution times must have the same length as the list of input sizes.")
        correlation = compute_pearson_correlation(sizes, execution_times)
        correlations.append(correlation)

    return correlations

def generate_comparison_table(all_values, title):
    sizes = [500, 1000, 2000, 4000, 8000, 16000, 32000, 64000, 80000, 100000]
    labels = ['Bubble Sort', 'Insertion Sort', 'Merge Sort', 'Quick Sort', 'Heap']

    df = pd.DataFrame(all_values, index=labels, columns=sizes)
    df = df.transpose()
    df.index.name = 'Input Size'
    table_title = f'{title} Comparisons'
    print(f'\n{table_title}\n')
    print(df.to_markdown())


if __name__ == "__main__":
    # Read the command-line argument
    if len(sys.argv) < 3:
        print("Usage: python graph.py '<list_of_lists>' '<title>'")
        sys.exit(1)
    
    try:
        exec_list = sys.argv[1]
        title = sys.argv[2]
        all_times = ast.literal_eval(exec_list)
        
        if not isinstance(all_times, list) or not all(isinstance(lst, list) for lst in all_times):
            raise ValueError("Data should be a list of lists.")
        
        print(title)
        if "Comparisions" in title:
            generate_comparison_table(all_times, title)
        else:
            plot_execution(all_times, title)
            if len(all_times) == 6 and title == "IncreasingOrderInputFile":
                plot_execution(all_times[1:], title)
            else:
                plot_execution(all_times[2:], title)
        
        correlations = compute_correlations(all_times)
        for i, corr in enumerate(correlations):
            print(f'Correlation between execution time and input size for algorithm {i+1}: {corr}')
    
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)