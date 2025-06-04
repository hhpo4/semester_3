import matplotlib.pyplot as plt
import numpy as np

# Чтение данных для полного набора
x_full, y_full_obs, y_full_pred = np.loadtxt('data_full.txt', unpack=True)
# Чтение данных для сокращенного набора
x_half, y_half_obs, y_half_pred = np.loadtxt('data_half.txt', unpack=True)

# Создаем фигуру с двумя подграфиками
plt.figure(figsize=(14, 6))

# Первый график - полные данные
plt.subplot(1, 2, 1)
plt.scatter(x_full, y_full_obs, label='Экспериментальные точки', color='red')
plt.plot(x_full, y_full_pred, label='Модель МНК (20 точек)', linewidth=2)
plt.xlabel('x')
plt.ylabel('y')
plt.title('Полный набор данных (20 точек)')
plt.legend()
plt.grid(True)

# Второй график - сокращенные данные
plt.subplot(1, 2, 2)
plt.scatter(x_half, y_half_obs, label='Экспериментальные точки', color='blue')
plt.plot(x_half, y_half_pred, label='Модель МНК (10 точек)', linewidth=2)
plt.xlabel('x')
plt.ylabel('y')
plt.title('Сокращенный набор данных (10 точек)')
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.savefig('comparison_plot.png')
plt.show()