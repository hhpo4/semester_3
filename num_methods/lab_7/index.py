import numpy as np
import matplotlib.pyplot as plt

# 1. Построение лемнискаты Бернулли
theta = np.linspace(0, 2*np.pi, 1000)  # Увеличиваем количество точек
r_squared = np.cos(2*theta)
r = np.sqrt(np.maximum(r_squared, 0))  # Обрабатываем отрицательные значения

# Переводим полярные координаты в декартовы
x_lemn = r * np.cos(theta)
y_lemn = r * np.sin(theta)

# 2. Построение второй кривой (xy² = 4(2 - y))
y_hyper = np.linspace(0.1, 3.9, 500)  # Избегаем y=0 (деление на 0) и выбираем адекватный диапазон
x_hyper = 4 * (2 - y_hyper) / (y_hyper**2)

# 3. Настройка графика
plt.figure(figsize=(10, 8))

# Построение лемнискаты (разделяем петли для наглядности)
plt.plot(x_lemn, y_lemn, 'b-', label='Лемниската Бернулли $(x^2+y^2)^2=x^2-y^2$')

# Построение второй кривой
plt.plot(x_hyper, y_hyper, 'r-', label='Кривая $xy^2=4(2-y)$')

# 4. Добавляем точки минимума (из вашего решения)
min_points = [
    (-0.456, 0.541),  # Точка на лемнискате
    (2.112, 0.723)    # Точка на гиперболе
]
plt.scatter(*zip(*min_points), color='green', s=100, label='Точки минимального расстояния')

# 5. Настраиваем отображение
plt.axhline(0, color='black', linewidth=0.5)
plt.axvline(0, color='black', linewidth=0.5)
plt.grid(True, linestyle='--', alpha=0.7)
plt.xlabel('x')
plt.ylabel('y')
plt.title('Минимальное расстояние между кривыми')
plt.legend()
plt.axis('equal')  # Важно для правильного масштаба
plt.xlim(-2, 4)    # Подбираем диапазон, чтобы было видно все кривые
plt.ylim(-1.5, 2.5)

# 6. Добавляем линию расстояния между точками
plt.plot([min_points[0][0], min_points[1][0]], 
         [min_points[0][1], min_points[1][1]], 
         'g--', linewidth=1, label='Минимальное расстояние')

plt.legend()
plt.tight_layout()
plt.show()