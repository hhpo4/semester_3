import numpy as np
import matplotlib.pyplot as plt

# Определяем функцию с использованием np.sin вместо math.sin
def f(x):
    return x**4 - x**2 * np.sin(x) - 2*x

# Создаем массив значений x
x = np.linspace(-2, 2, 600)

# Вычисляем соответствующие значения y
y = f(x)

# Создаем график
plt.figure(figsize=(10, 6))
plt.plot(x, y, label=r'$f(x) = x^4 - x^2 \cdot \sin(x) - 2x$', color='blue')
plt.axhline(0, color='green', lw=1.5, ls='--')  # Горизонтальная линия y=0
plt.axvline(0, color='red', lw=1.5, ls='--')  # Вертикальная линия x=0
plt.title('График функции $f(x) = x^4 - x^2 \cdot \sin(x) - 2x$')
plt.xlabel('x')
plt.ylabel('f(x)')
plt.grid()
plt.legend()
plt.show()