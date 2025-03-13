@echo off
setlocal

REM Проверка наличия двух параметров
if "%~1"=="" (
    echo Ошибка: Не указан файл для исключения.
    exit /b 1
)
if "%~2"=="" (
    echo Ошибка: Не указан каталог-приемник.
    exit /b 1
)

REM Установка переменных
set "exclude_file=%~nx1"
set "destination_dir=%~2"

REM Проверка существования каталога-приемника
if not exist "%destination_dir%" (
    echo Ошибка: Каталог-приемник "%destination_dir%" не существует.
    exit /b 1
)

REM Копирование файлов
for %%f in (*.*) do (
    if /i not "%%f"=="%exclude_file%" (
        echo Копирование файла: %%f
        xcopy /D /Y "%%f" "%destination_dir%"
        if errorlevel 1 (
            echo Ошибка при копировании файла: %%f
            exit /b 1
        )
    )
)

echo Копирование завершено успешно.
endlocal
