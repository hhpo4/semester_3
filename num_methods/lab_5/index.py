import matplotlib.pyplot as plt
import numpy as np

def read_data_from_file(filename):
    data = []
    try:
        with open(filename, 'r') as f:
            for line in f:
                line = line.strip()
                if not line:
                    continue
                
                try:
                    parts = list(map(float, line.split()))
                    if len(parts) != 3:
                        raise ValueError("Строка должна содержать ровно 3 числа")
                    data.append((parts[0], parts[1], parts[2]))
                except ValueError as e:
                    print(f"Пропускаем строку '{line}' из-за ошибки: {str(e)}")
                    continue
    
    except FileNotFoundError:
        print(f"Ошибка: Файл '{filename}' не найден.")
    except Exception as e:
        print(f"Неожиданная ошибка при чтении файла: {str(e)}")
    
    return data

def analytical_solution(x_values):
    """Аналитическое решение тестовой системы"""
    y = [-np.sin(x) for x in x_values]
    z = [0 for _ in x_values]
    return y, z

def plot_combined_results():
    # Чтение данных
    main_rk2 = read_data_from_file("rk2_results.txt")
    main_rk4 = read_data_from_file("rk4_results.txt")
    test_rk2 = read_data_from_file("rk2_test_results.txt")
    test_rk4 = read_data_from_file("rk4_test_results.txt")

    # Создаем фигуру с двумя графиками
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6))

    # Цвета и стили линий
    color_main = '#1f77b4'  # Синий
    color_test = '#ff7f0e'   # Оранжевый
    color_analytical = '#2ca02c'  # Зеленый
    line_rk2 = '-'          # Сплошная линия для RK2
    line_rk4 = '--'         # Пунктир для RK4
    line_analytical = ':'   # Точечная линия для аналитического решения

    # Общие x-значения для аналитического решения
    x_analytical = np.linspace(0, 0.5, 100)
    y_analytical, z_analytical = analytical_solution(x_analytical)

    # График 1: x от z
    if main_rk2:
        x, y, z = zip(*main_rk2)
        ax1.plot(x, z, color=color_main, linestyle=line_rk2, label='Основная (RK2)')
    if main_rk4:
        x, y, z = zip(*main_rk4)
        ax1.plot(x, z, color='#f00', linestyle=line_rk4, label='Основная (RK4)')
    if test_rk2:
        x, y, z = zip(*test_rk2)
        ax1.plot(x, z, color=color_test, linestyle=line_rk2, label='Тестовая (RK2)')
    if test_rk4:
        x, y, z = zip(*test_rk4)
        ax1.plot(x, z, color='#0f0', linestyle=line_rk4, label='Тестовая (RK4)')
    
    # Аналитическое решение z(x) для тестовой системы
    ax1.plot(x_analytical, z_analytical, color=color_analytical, 
             linestyle=line_analytical, label='Аналитическое z(x)')

    ax1.set_xlabel('x')
    ax1.set_ylabel('z')
    ax1.set_title('Зависимость z от x')
    ax1.grid(True)
    ax1.legend()

    # График 2: x от y
    if main_rk2:
        x, y, z = zip(*main_rk2)
        ax2.plot(x, y, color=color_main, linestyle=line_rk2, label='Основная (RK2)')
    if main_rk4:
        x, y, z = zip(*main_rk4)
        ax2.plot(x, y, color='#f00', linestyle=line_rk4, label='Основная (RK4)')
    if test_rk2:
        x, y, z = zip(*test_rk2)
        ax2.plot(x, y, color=color_test, linestyle=line_rk2, label='Тестовая (RK2)')
    if test_rk4:
        x, y, z = zip(*test_rk4)
        ax2.plot(x, y, color='#0f0', linestyle=line_rk4, label='Тестовая (RK4)')
    
    # Аналитическое решение y(x) для тестовой системы
    ax2.plot(x_analytical, y_analytical, color=color_analytical, 
             linestyle=line_analytical, label='Аналитическое y(x)')

    ax2.set_xlabel('x')
    ax2.set_ylabel('y')
    ax2.set_title('Зависимость y от x')
    ax2.grid(True)
    ax2.legend()

    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    plot_combined_results()