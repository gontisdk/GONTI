@echo off
tree /f /a > struct.txt

set /p userInput=Enter command (Build/Debug): 

if /i "%userInput%"=="build" (
    goto build
) else if /i "%userInput%"=="b" (
    goto build
) else if /i "%userInput%"=="debug" (
    goto debug
) else if /i "%userInput%"=="d" (
    goto debug
) else (
    echo Invalid value exiting...
    goto end
)

:build
cd _Build/Scripts/win/
start main.bat
goto end

:debug
cd "Tests/Unit/TestbedHost/win/"
start Debug.bat

:end