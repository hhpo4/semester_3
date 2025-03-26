#!/bin/bash

# Читаем переменные a и b
echo -n "Введите a: "
read a
echo -n "Введите b: "
read b

# Вычисления
sum=$((a + b))
prod=$((a * bb))
sub=$((a - b))
div=$((a / b))
rem=$((a % b))

# Вывод результатов
echo "Сумма: $sum"
echo "Произведение: $prod"
echo "Разность: $sub"
echo "Частное: $div"
echo "Остаток: $rem"
