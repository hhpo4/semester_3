import numpy as np
import matplotlib.pyplot as plt
import sys

def plot_results(prefix):
    # Загрузка данных
    t, f, y_opt = np.loadtxt(f'{prefix}_signal.txt', unpack=True)
    t_err, err = np.loadtxt(f'{prefix}_error.txt', unpack=True)
    freq, Ak, phi_k = np.loadtxt(f'{prefix}_spectrum.txt', unpack=True)
    m, Q = np.loadtxt(f'{prefix}_q.txt', unpack=True)
    Ef = np.loadtxt(f'{prefix}_energy.txt')
    
    # Настройка стиля
    plt.style.use('seaborn-v0_8-whitegrid')
    fig = plt.figure(figsize=(14, 10))
    fig.suptitle('Аппроксимация рядом Фурье: ' + ('Периодическая функция' if 'periodic' in prefix else 'Непериодическая функция'), fontsize=16)

    # График сигнала
    ax1 = plt.subplot(221)
    ax1.plot(t, f, 'b-', linewidth=2, label='Исходный')
    ax1.plot(t, y_opt, 'r--', linewidth=1.5, label='Аппроксимация')
    ax1.set_title('Сравнение сигналов')
    ax1.set_xlabel('Время')
    ax1.set_ylabel('Амплитуда')
    ax1.legend()
    ax1.grid(True, linestyle='--', alpha=0.7)

    # График ошибки
    ax2 = plt.subplot(222)
    ax2.plot(t_err, err, 'g-', linewidth=1.5)
    ax2.set_title('Ошибка аппроксимации')
    ax2.set_xlabel('Время')
    ax2.set_ylabel('Ошибка')
    ax2.grid(True, linestyle='--', alpha=0.7)

    # Амплитудный спектр
    ax3 = plt.subplot(223)
    ax3.stem(freq, Ak, 'b-', markerfmt='bo', basefmt=' ')
    ax3.set_title('Амплитудный спектр')
    ax3.set_xlabel('Частота (рад/с)')
    ax3.set_ylabel('Амплитуда')
    ax3.set_xlim([0, freq[-1]*1.1])
    ax3.grid(True, linestyle='--', alpha=0.7)

    # Зависимость Q(m)
    ax4 = plt.subplot(224)
    ax4.plot(m, Q, 'm-', linewidth=2)
    ax4.set_title('Зависимость дисперсии от числа гармоник')
    ax4.set_xlabel('Число гармоник (m)')
    ax4.set_ylabel('Дисперсия (Q)')
    ax4.grid(True, linestyle='--', alpha=0.7)
    
    # Отображение энергии
    plt.figtext(0.5, 0.01, f'Энергия сигнала: {Ef:.4f}', ha='center', fontsize=12)
    
    plt.tight_layout(rect=[0, 0.03, 1, 0.95])
    plt.savefig(f'{prefix}_results.png', dpi=150)
    plt.close()

if __name__ == "__main__":
    prefixes = ['periodic', 'nonperiodic']
    for prefix in prefixes:
        plot_results(prefix)