import numpy as np
import matplotlib.pyplot as plt

# Определяем функцию
def f(x):
    return x**5 - 3*x**2 + 1

# Создаем массив значений x
x = np.linspace(-2, 2, 600)

# Вычисляем соответствующие значения y
y = f(x)

# Создаем график
plt.figure(figsize=(10, 6))
plt.plot(x, y, label=r'$f(x) = x^5 - 3x^2 + 1$', color='blue')
plt.axhline(0, color='green', lw=1.5, ls='--')  # Горизонтальная линия y=0
plt.axvline(0, color='red', lw=1.5, ls='--')  # Вертикальная линия x=0
plt.title('График функции $f(x) = x^5 - 3x^2 + 1$')
plt.xlabel('x')
plt.ylabel('f(x)')
plt.grid()
plt.legend()
plt.ylim(-10, 10)  # Ограничиваем ось y для лучшего отображения
plt.show()
