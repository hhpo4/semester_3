import matplotlib.pyplot as plt

# Функция для чтения данных из файла
def read_data(file_path):
    x_values = []
    d1_values = []  # Левая производная
    d2_values = []  # Центральная производная
    d3_values = []  # Правая производная
    analytical_values = []  # Аналитическая производная

    with open(file_path, 'r') as f:
        for line in f:
            if line.startswith("x"):  # Пропускаем заголовок
                continue
            data = line.split()
            if len(data) < 5:
                continue
            x_values.append(float(data[0]))
            d1_values.append(float(data[1]))
            d2_values.append(float(data[2]))
            d3_values.append(float(data[3]))
            analytical_values.append(float(data[4]))
    
    return x_values, d1_values, d2_values, d3_values, analytical_values

# Функция для построения графика
def plot_data(x_values, d1_values, d2_values, d3_values, analytical_values):
    plt.figure(figsize=(12, 6))
    plt.plot(x_values, d1_values, label="Левая производная", linestyle='--', color='blue')
    plt.plot(x_values, d2_values, label="Центральная производная", linestyle='-.', color='green')
    plt.plot(x_values, d3_values, label="Правая производная", linestyle=':', color='red')
    plt.plot(x_values, analytical_values, label="Аналитическая производная", linestyle='-', color='black', linewidth=2)
    
    plt.xlabel("x")
    plt.ylabel("Производная")
    plt.title(f"Сравнение численных и аналитических производных (Количество узлов: {len(x_values)})")
    plt.legend()
    plt.grid(True)
    plt.show()

# Основной блок кода
if __name__ == "__main__":
    file_path = "data.txt"
    x_values, d1_values, d2_values, d3_values, analytical_values = read_data(file_path)
    plot_data(x_values, d1_values, d2_values, d3_values, analytical_values)
