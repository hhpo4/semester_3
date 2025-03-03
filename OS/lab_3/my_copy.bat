@echo off
rem MYCOPY.BAT копирует произвольное количество файлов
rem в каталог
rem используется следующий синтаксис:
rem mycopy dir file1 file2 file3 ...
set todir=%1
:getfile
shift
if "%1"=="" goto end
copy %1 %todir%
goto getfile
:end
set todir=
echo Работа завершена

rem my_copy .\folder2 .\folder1\test.txt .\folder1\test1.txt