import matplotlib.pyplot as plt
import numpy as np

def f(x):
    return np.cos(x)

# Загрузка результатов из файла
min_points = []
max_points = []
with open("extrema_results.txt", "r") as file:
    for line in file:
        parts = line.strip().split()
        a, b = float(parts[0]), float(parts[1])
        x_min, f_min = float(parts[3]), float(parts[4])
        x_max, f_max = float(parts[6]), float(parts[7])
        min_points.append((x_min, f_min))
        max_points.append((x_max, f_max))

# Отрисовка графика
x = np.linspace(0, 6, 1000)
y = f(x)

plt.figure(figsize=(10, 6))
plt.plot(x, y, label='f(x) = cos(x)', color='blue')

# Добавляем экстремумы
for i, (x_min, f_min) in enumerate(min_points):
    plt.plot(x_min, f_min, 'ro', label='Минимум' if i == 0 else "")
for i, (x_max, f_max) in enumerate(max_points):
    plt.plot(x_max, f_max, 'go', label='Максимум' if i == 0 else "")

plt.title("График функции и найденные экстремумы")
plt.xlabel("x")
plt.ylabel("f(x)")
plt.xlim(0, 6)
plt.ylim(-1.5, 1.5)
plt.legend()
plt.grid(True)
plt.show()