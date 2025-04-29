import matplotlib.pyplot as plt

filename_rk2 = "rk2_results.txt"
filename_rk4 = "rk4_results.txt"

def read_data_from_file(filename=filename_rk4):
    data = []
    try:
        with open(filename, 'r') as f:
            for line in f:
                line = line.strip()
                if not line:  # Пропускаем пустые строки
                    continue
                
                try:
                    # Разделяем строку по пробелам и преобразуем в числа
                    parts = list(map(float, line.split()))
                    if len(parts) != 3:
                        raise ValueError("Строка должна содержать ровно 3 числа")
                    
                    x, y, z = parts
                    data.append((x, y, z))
                
                except ValueError as e:
                    print(f"Пропускаем строку '{line}' из-за ошибки: {str(e)}")
                    continue
    
    except FileNotFoundError:
        print(f"Ошибка: Файл '{filename}' не найден.")
    except Exception as e:
        print(f"Неожиданная ошибка при чтении файла: {str(e)}")
    
    return data

def plot_data(data, title="Результаты Рунге-Кутты"):
   
    if not data:
        print("Нет данных для построения графика.")
        return

    try:
        x_values, y_values, z_values = zip(*data)
        
        plt.figure(figsize=(10, 6))
        plt.plot(x_values, y_values, label='y(x)')
        plt.plot(x_values, z_values, label='z(x)')
        
        plt.xlabel('x')
        plt.ylabel('y, z')
        plt.title(title)
        plt.grid(True)
        plt.legend()
        plt.show()
    
    except Exception as e:
        print(f"Ошибка при построении графика: {str(e)}")

# Пример использования
if __name__ == "__main__":  # Исправлено здесь
    data = read_data_from_file()
    plot_data(data)