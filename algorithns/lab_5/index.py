import matplotlib.pyplot as plt
import math

def read_data_from_file(filename):
    sizes, insert_times, remove_times = [], [], []
    with open(filename, 'r') as file:
        for line in file:
            values = line.strip().split()
            if len(values) == 3:
                sizes.append(int(values[0]))
                insert_times.append(float(values[1]))
                remove_times.append(float(values[2]))
    return sizes, insert_times, remove_times

def theoretical_insert_time(n):
    return 1  # Вставка в конец списка O(1)

def theoretical_remove_time(n):
    return n

filename = "performance.txt"
sizes, experimental_insert, experimental_remove = read_data_from_file(filename)

theoretical_insert = [theoretical_insert_time(n) for n in sizes]
theoretical_remove = [theoretical_remove_time(n) for n in sizes]

fig, axes = plt.subplots(2, 2, figsize=(12, 10))
fig.suptitle("Анализ производительности двусвязного списка")

# Экспериментальное время вставки
axes[0, 0].plot(sizes, theoretical_insert, label="Insert (Experimental)", marker='o', linestyle='--')
axes[0, 0].set_xlabel("Размер списка")
axes[0, 0].set_ylabel("Время (сек)")
axes[0, 0].set_title("Экспериментальное время вставки")
axes[0, 0].legend()
axes[0, 0].grid(True)

# Экспериментальное время удаления
axes[0, 1].plot(sizes, experimental_remove, label="Remove (Experimental)", marker='s', linestyle='--')
axes[0, 1].set_xlabel("Размер списка")
axes[0, 1].set_ylabel("Время (сек)")
axes[0, 1].set_title("Экспериментальное время удаления")
axes[0, 1].legend()
axes[0, 1].grid(True)

# Теоретическое время вставки
axes[1, 0].plot(sizes, theoretical_insert, label="Insert (Theoretical O(1))", marker='x', linestyle=':')
axes[1, 0].set_xlabel("Размер списка")
axes[1, 0].set_ylabel("Время (сек)")
axes[1, 0].set_title("Теоретическое время вставки")
axes[1, 0].legend()
axes[1, 0].grid(True)

# Теоретическое время удаления
axes[1, 1].plot(sizes, theoretical_remove, label="Remove (Theoretical O(n))", marker='x', linestyle=':')
axes[1, 1].set_xlabel("Размер списка")
axes[1, 1].set_ylabel("Время (сек)")
axes[1, 1].set_title("Теоретическое время удаления")
axes[1, 1].legend()
axes[1, 1].grid(True)

plt.tight_layout()
plt.show()
