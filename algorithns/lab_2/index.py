import matplotlib.pyplot as plt
import numpy as np
import math

n_values = [6, 7, 8, 9, 10]
theoretical_function = lambda n: math.factorial(n)

with open("execution_times.txt", "r") as file:
    lines = file.readlines()

time_results = [np.array(list(map(float, line.strip().split(',')))) for line in lines]

theoretical_results = [theoretical_function(n) for n in n_values]

colors = ['red', 'green', 'blue', 'purple']
labels = ['int', 'float', 'double', 'char']

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6))

# График для теоретических результатов
ax1.plot(n_values, theoretical_results, 'k--', label='Theoretical O(N^4)')
ax1.set_xlabel('Размер N')
ax1.set_ylabel('Теоретическое время (усл. ед.)', color='black')
ax1.tick_params(axis='y', labelcolor='black')
ax1.set_title('Теоретическая сложность алгоритма')
ax1.grid(True)
ax1.legend()

# График для экспериментальных результатов
for i, (data, color, label) in enumerate(zip(time_results, colors, labels)):
    ax2.plot(n_values, data, 'o-', color=color, label=label)

ax2.set_xlabel('Размер N')
ax2.set_ylabel('Время работы (с)')
ax2.set_title('Экспериментальные результаты')
ax2.grid(True)
ax2.legend()

plt.tight_layout()
plt.show()

#g++ index.cpp -o index.exe; ./index.exe; python index.py