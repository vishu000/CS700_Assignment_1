import random

# Function to create random data sets
def generate_random_cases(count):
    cases = [random.randint(0, 1000000) for _ in range(count)]
    return "\n".join(map(str, cases))

def generate_best_cases(count):
    cases = sorted([random.randint(0, 1000000) for _ in range(count)])
    return "\n".join(map(str, cases))

def generate_worst_cases(count):
    cases = sorted([random.randint(0, 1000000) for _ in range(count)], reverse=True)
    return "\n".join(map(str, cases))

# Sizes to generate
input_sizes = [500, 1000, 2000, 4000, 8000, 16000, 32000, 64000, 80000, 100000]

# Generate random test cases
with open("Random.txt", "w") as random_file:
    for size in input_sizes:
        random_file.write(f"Test cases for n={size}:\n")
        random_file.write(generate_random_cases(size) + "\n\n")

# Generate best-case test cases
with open("Increasing.txt", "w") as best_file:
    for size in input_sizes:
        best_file.write(f"Test cases for n={size}:\n")
        best_file.write(generate_best_cases(size) + "\n\n")

# Generate worst-case test cases
with open("Decreasing.txt", "w") as worst_file:
    for size in input_sizes:
        worst_file.write(f"Test cases for n={size}:\n")
        worst_file.write(generate_worst_cases(size) + "\n\n")
