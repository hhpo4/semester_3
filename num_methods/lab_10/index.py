import matplotlib.pyplot as plt
import numpy as np
import os
import glob

# Создаем фигуру с двумя подграфиками
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 12), sharex=True)

# Загрузка аналитического решения
analytic_data = np.loadtxt('analytic_solution.txt')
x_analytic = analytic_data[:, 0]
y_analytic = analytic_data[:, 1]
ax1.plot(x_analytic, y_analytic, label='Analytical Solution', linewidth=2, color='black')

# Загрузка финального численного решения
final_data = np.loadtxt('final_solution.txt')
x_final = final_data[:, 0]
y_final = final_data[:, 1]
ax1.plot(x_final, y_final, '--', label='Final Numerical Solution', linewidth=2, color='red')

# Настройки первого подграфика (аналитическое и численное решения)
ax1.set_ylabel('y(x)')
ax1.set_title('Analytical vs Numerical Solution')
ax1.grid(True)
ax1.legend()

# Визуализация всех "выстрелов" (shot_*.txt и changed_*.txt) на втором подграфике
colors = plt.cm.rainbow(np.linspace(0, 1, 100))  # Цветовая палитра

# Отрисовка shot_*.txt
for i, filename in enumerate(sorted(glob.glob('shot_*.txt'))):
    data = np.loadtxt(filename)
    x = data[:, 0]
    y = data[:, 1]
    ax2.plot(x, y, ':', color=colors[i], alpha=0.5, linewidth=1, label='Shots' if i == 0 else "")

# Отрисовка changed_*.txt
for i, filename in enumerate(sorted(glob.glob('changed_*.txt'))):
    data = np.loadtxt(filename)
    x = data[:, 0]
    y = data[:, 1]
    ax2.plot(x, y, '-.', color=colors[i], alpha=0.7, linewidth=1, label='Changed' if i == 0 else "")

# Настройки второго подграфика (итерации)
ax2.set_xlabel('x')
ax2.set_ylabel('y(x)')
ax2.set_title('Shooting Iterations')
ax2.grid(True)
ax2.legend()

plt.tight_layout()
plt.savefig('combined_plot.png', dpi=300, bbox_inches='tight')
plt.show()