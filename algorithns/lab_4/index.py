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

def theoretical_function(n):
    return math.log2(n) if n > 0 else 0

filename = "execution_times.txt"
sizes, experimental_data = read_data_from_file(filename)

theoretical_values = [theoretical_function(n) for n in sizes]

plt.figure(figsize=(12, 6))

# Построение графиков
plt.plot(sizes, experimental_data["sequential_search"], label="Sequential Search", marker='o', linestyle='--')
plt.plot(sizes, experimental_data["binary_search"], label="Binary Search", marker='s', linestyle='-')
plt.plot(sizes, theoretical_values, label="Theoretical Log2(n)", marker='x', linestyle=':')

# Добавление подписей
plt.xlabel("Размер массива")
plt.ylabel("Время выполнения")
plt.title("Сравнение поисковых алгоритмов")
plt.legend()
plt.grid(True)

# Показать график
plt.show()