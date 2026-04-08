@echo off
tree /f /a > tree.txt

rem Wyczyść output
cls

echo "What wanna you do:"
echo "1 or b - Build engine"
echo "2 or tb - Build testbed"
echo "3 or ts - Build tests"
set /p userInput=Enter command (Build/Testbed/Debug): 

if /i "%userInput%"=="build" (
    goto build
) else if /i "%userInput%"=="b" (
    goto build
) else if /i "%userInput%"=="1" (
    goto build
) else if /i "%userInput%"=="testbed" (
    goto testbed
) else if /i "%userInput%"=="tb" (
    goto testbed
) else if /i "%userInput%"=="2" (
    goto testbed
) else if /i "%userInput%"=="tests" (
    goto tests
) else if /i "%userInput%"=="ts" (
    goto tests
) else if /i "%userInput%"=="3" (
    goto tests
) else (
    echo Invalid value exiting...
    goto end
)

:build
cd GONTI-SDK/Build/Scripts/win32/
start main.bat
goto end

:testbed
rem cd Testbed/BuildScripts/win/
call Testbed/BuildScripts/win/build.bat
goto end

:tests
rem cd Tests/BuildScripts/win/
call Tests/BuildScripts/win/build.bat
goto end

:end