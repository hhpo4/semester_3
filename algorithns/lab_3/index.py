import matplotlib.pyplot as plt
import numpy as np
import math

def read_data_from_file(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()
    
    sort_type = lines[0].strip().lower()
    
    data = {"random": [], "reversed": [], "median_as_last": []}
    sizes = []
    
    for line in lines[1:]:
        values = line.strip().split(',')
        sizes.append(int(values[0]))
        data["random"].append(float(values[1]))
        data["reversed"].append(float(values[2]))
        data["median_as_last"].append(float(values[3]))
    
    return sort_type, sizes, data

n_linear = lambda n: n
n_log_n = lambda n: n * math.log2(n) if n > 0 else 0
n_times_n = lambda n: n**2  

theoretical_functions = {
    "insert": {"best": n_linear, "worst": n_times_n, "avg": n_log_n},
    "quick": {"avg": n_log_n, "worst": n_times_n},
    "bubble": {"worst": n_times_n}
}

def plot_graphs(sort_type, sizes, data):
    sorted_indices = np.argsort(sizes)
    sizes = np.array(sizes)[sorted_indices]
    for key in data:
        data[key] = np.array(data[key])[sorted_indices]

    fig, axes = plt.subplots(1, 4, figsize=(20, 5))
    fig.suptitle(f'Сравнение экспериментальных данных и теоретических кривых для {sort_type.capitalize()} сортировки')

    labels = ["Случайный порядок", "Обратный порядок", "Медиана в конце"]
    keys = ["random", "reversed", "median_as_last"]
    colors = ['b', 'r', 'g']

    max_experimental = max(max(data["random"]), max(data["reversed"]), max(data["median_as_last"]))

    for i in range(3):
        axes[i].plot(sizes, data[keys[i]], 'o-', color=colors[i], label=labels[i])
        axes[i].set_title(labels[i])
        axes[i].set_xlabel('Размер массива')
        axes[i].set_ylabel('Время (сек)')
        axes[i].legend()
        axes[i].grid(True)
        axes[i].set_ylim(0, max_experimental * 1.1)
    
    x_theoretical = np.linspace(0, 100, 100)
    theoretical_function_avg = theoretical_functions.get(sort_type, {}).get("avg", None)
    theoretical_function_worst = theoretical_functions.get(sort_type, {}).get("worst", None)
    theoretical_function_best = theoretical_functions.get(sort_type, {}).get("best", None)

    if theoretical_function_avg:
        axes[3].plot(x_theoretical, [theoretical_function_avg(n) for n in x_theoretical], 'm--', label='Средний случай')
    if theoretical_function_worst:
        axes[3].plot(x_theoretical, [theoretical_function_worst(n) for n in x_theoretical], 'c--', label='Худший случай')
    if theoretical_function_best:
        axes[3].plot(x_theoretical, [theoretical_function_best(n) for n in x_theoretical], 'y--', label='Лучший случай')

    axes[3].set_title('Теоретические оценки сложности')
    axes[3].set_xlabel('Размер массива (абстрактный масштаб)')
    axes[3].set_ylabel('Оценка сложности')
    axes[3].legend()
    axes[3].grid(True)

    plt.tight_layout()
    plt.show()

filename = "execution_times.txt"
sort_type, sizes, data = read_data_from_file(filename)
plot_graphs(sort_type, sizes, data)