@echo off

rem Uruchom plik lub wyjdź

echo(
echo(
echo ========================================================================================================================
echo ========================================================================================================================
echo ========================================================================================================================
echo(
echo(
echo(

set /p userInput=Enter command (run/quit): 

if /i "%userInput%"=="run" (
    goto run
) else if /i "%userInput%"=="r" (
    goto run
) else if /i "%userInput%"=="quit" (
    echo Exiting...
    goto end
) else (
    echo Invalid value exiting...
    goto end
)

:run
echo(
echo(
echo Starting program...
echo(
echo ================================
echo ====    START OF PROGRAM    ====
echo ================================
echo(
echo(

%assembly%.exe

echo(
echo(
echo(
echo ================================
echo ====     END OF PROGRAM     ====
echo ================================
echo(

goto end
:end