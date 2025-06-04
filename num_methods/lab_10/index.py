import matplotlib.pyplot as plt
import numpy as np
import os

# Сравнение аналитического и численного решений
analytic_data = np.loadtxt('analytic_solution.txt')
x_analytic = analytic_data[:, 0]
y_analytic = analytic_data[:, 1]

final_data = np.loadtxt('final_solution.txt')
x_final = final_data[:, 0]
y_final = final_data[:, 1]

plt.figure(figsize=(10, 6))
plt.plot(x_analytic, y_analytic, label='Analytical Solution', linewidth=2)
plt.plot(x_final, y_final, '--', label='Numerical Solution', linewidth=2)
plt.xlabel('x')
plt.ylabel('y(x)')
plt.title('Comparison of Analytical and Numerical Solutions')
plt.legend()
plt.grid(True)
plt.savefig('comparison.png')
plt.show()

# Визуализация всех "выстрелов"
plt.figure(figsize=(10, 6))
for i in range(100):
    filename = f'shot_{i}.txt'
    if os.path.exists(filename):
        data = np.loadtxt(filename)
        x = data[:, 0]
        y = data[:, 1]
        plt.plot(x, y, label=f'Shot {i+1}')
plt.xlabel('x')
plt.ylabel('y(x)')
plt.title('Trajectories from Shooting Method Iterations')
plt.grid(True)
plt.legend()
plt.savefig('shooting_iterations.png')
plt.show()