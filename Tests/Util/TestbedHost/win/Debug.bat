rem Debuging Engine
@echo off
SetLocal EnableDelayedExpansion

echo WAITING! If you do not compiled program please do it first
set /p userInput=You compiled? (Yes/No): 

if /i "%userInput%"=="yes" (
    goto debug
) else if /i "%userInput%"=="y" (
    goto debug
) else if /i "%userInput%"=="no" (
    goto end
) else if /i "%userInput%"=="n" (
    goto end
) else (
    echo Invalid value exiting...
    goto end
)

:debug
set exeFile=../../../_Build/win/testbed/testbed.exe

if not exist "%exeFile%" (
  echo ERROR: Plik "%exeFile%" nie istnieje. Skonpiluj najpierw projekt.
  goto end
)

gdb "%exeFile%"
goto end

:end
pause
exit