@echo off
setlocal enabledelayedexpansion

if "%~1"=="" (
    echo Использование: %~nx0 ^<каталог1^> [каталог2 ...]
    exit /b 1
)

set "logfile=delete_tmp.log"
echo Удаление файлов .TMP - %DATE% %TIME% > "%logfile%"

for %%D in (%*) do (
    if exist "%%D" (
        for %%F in ("%%D\*.TMP") do (
            echo Удаляется: %%F >> "%logfile%"
            del "%%F" || echo Ошибка удаления %%F >> "%logfile%"
        )
    ) else (
        echo Каталог не найден: %%D >> "%logfile%"
    )
)

echo Завершено. Лог: %logfile%
exit /b 0
