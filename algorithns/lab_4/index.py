import matplotlib.pyplot as plt
import math

def read_data_from_file(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()

    data = {
        "sequential_search": [],
        "binary_search": [],
    }

    sizes = [] 

    for line in lines:
        values = line.strip().split(',')
        sizes.append(int(values[0])) 
        data["sequential_search"].append(float(values[1])) 
        data["binary_search"].append(float(values[2])) 

    return sizes, data

def theoretical_binary_search(n):
    return math.log2(n) if n > 0 else 0

def theoretical_sequential_search(n):
    return n

filename = "execution_times.txt"
sizes, experimental_data = read_data_from_file(filename)

theoretical_binary_values = [theoretical_binary_search(n) for n in sizes]
theoretical_sequential_values = [theoretical_sequential_search(n) for n in sizes]

fig, axes = plt.subplots(1, 2, figsize=(15, 5))  # Уменьшил количество осей до 2
fig.suptitle("Сравнение экспериментальных и теоретических данных для поисковых алгоритмов")

# Построение графиков
# График для последовательного поиска
axes[0].plot(sizes, experimental_data["sequential_search"], label="Sequential Search (Experimental)", marker='o', linestyle='--')
axes[0].plot(sizes, theoretical_sequential_values, label="Sequential Search (Theoretical O(n))", marker='x', linestyle=':')
axes[0].set_xlabel("Размер массива")
axes[0].set_ylabel("Время выполнения")
axes[0].set_title("Последовательный поиск")
axes[0].legend()
axes[0].grid(True)

# График для бинарного поиска
axes[1].plot(sizes, experimental_data["binary_search"], label="Binary Search (Experimental)", marker='s', linestyle='-')
axes[1].plot(sizes, theoretical_binary_values, label="Binary Search (Theoretical O(log2(n)))", marker='x', linestyle=':')
axes[1].set_xlabel("Размер массива")
axes[1].set_ylabel("Время выполнения")
axes[1].set_title("Бинарный поиск")
axes[1].legend()
axes[1].grid(True)

# Показать график
plt.tight_layout()
plt.show()