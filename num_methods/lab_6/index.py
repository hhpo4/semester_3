import matplotlib.pyplot as plt
import numpy as np

def f(x):
    return np.sin(x)**2 - 2 * np.tan(x)

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
x = np.linspace(0, 2, 1000)
y = f(x)

plt.figure(figsize=(10, 6))
plt.plot(x, y, label='f(x) = sin²(x) - 2tan(x)', color='blue')
plt.axvline(np.pi / 2, color='gray', linestyle='--', label='Разрыв tan(x)')

# Добавляем экстремумы
for x_min, f_min in min_points:
    plt.plot(x_min, f_min, 'ro', label='Минимум' if x_min == min_points[0][0] else "")
for x_max, f_max in max_points:
    plt.plot(x_max, f_max, 'go', label='Максимум' if x_max == max_points[0][0] else "")

plt.ylim(-10, 10)
plt.xlim(0, 2)
plt.title("График функции и найденные экстремумы")
plt.xlabel("x")
plt.ylabel("f(x)")
plt.legend()
plt.grid(True)
plt.show()
