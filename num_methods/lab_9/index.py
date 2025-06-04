import numpy as np
import matplotlib.pyplot as plt

# Чтение данных из файла
with open('spline_coeffs.txt', 'r') as f:
    N = int(f.readline())
    x_nodes = list(map(float, f.readline().split()))
    coeffs = []
    for _ in range(N):
        row = list(map(float, f.readline().split()))
        coeffs.append(row)

# Функция для вычисления значения сплайна
def spline_value(x, nodes, coeffs):
    for i in range(len(nodes)-1):
        if nodes[i] <= x <= nodes[i+1]:
            a, b, c, d = coeffs[i]
            dx = x - nodes[i]
            return a + b*dx + c*dx**2 + d*dx**3
    return None

# Создание фигуры с двумя подграфиками
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6))

# Основной график (без продленного сплайна)
x_true = np.linspace(-1, 1, 1000)
y_true = np.cosh(x_true)
ax1.plot(x_true, y_true, 'k--', label='Исходная функция (ch(x))')

# Отрисовка сплайнов разными цветами (только на своем интервале)
colors = plt.cm.tab10(np.linspace(0, 1, N))
for i in range(N):
    x_seg = np.linspace(x_nodes[i], x_nodes[i+1], 100)
    a, b, c, d = coeffs[i]
    y_seg = a + b*(x_seg - x_nodes[i]) + c*(x_seg - x_nodes[i])**2 + d*(x_seg - x_nodes[i])**3
    ax1.plot(x_seg, y_seg, color=colors[i], label=f'Сплайн {i+1}')

# Узлы интерполяции
ax1.scatter(x_nodes, np.cosh(x_nodes), color='black', zorder=5, label='Узлы')

ax1.set_xlabel('x')
ax1.set_ylabel('y')
ax1.set_title('Интерполяция кубическим сплайном для ch(x)')
ax1.legend()
ax1.grid(True)

# Второй график (продленный сплайн)
mid = N // 2  # Выбираем средний сплайн
a, b, c, d = coeffs[mid]
x_ext = np.linspace(-1, 1, 500)  # Продлеваем на весь отрезок [-1, 1]
y_ext = a + b*(x_ext - x_nodes[mid]) + c*(x_ext - x_nodes[mid])**2 + d*(x_ext - x_nodes[mid])**3

ax2.plot(x_true, y_true, 'k--', label='Исходная функция (ch(x))')
ax2.plot(x_ext, y_ext, 'r-', linewidth=2, label='Продленный сплайн (5)')
ax2.scatter(x_nodes, np.cosh(x_nodes), color='black', zorder=5, label='Узлы')

ax2.set_xlabel('x')
ax2.set_ylabel('y')
ax2.set_title('Продленный сплайн на [-1, 1]')
ax2.legend()
ax2.grid(True)

plt.tight_layout()
plt.show()

# Вычисление значения в точке x*
x_star = 0.25
y_star = spline_value(x_star, x_nodes, coeffs)
print(f'Значение сплайна в x*={x_star}: {y_star}')
print(f'Истинное значение в x*={x_star}: {np.cosh(x_star)}')