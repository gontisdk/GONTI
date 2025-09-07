@echo off
title Clang Compiler Console by OXlatorty
rem Build Everything

echo Build Everything...

rem call makeEngine.bat
cd ../../../GONTI/Source/Utilities/Build/win
call makeEngine.bat

echo(
echo(

rem if %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)
if %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && goto end)

echo(
echo(
echo(
echo(

rem call makeTestbed.bat
cd ../../../../../Tests/Util/TestbedHost/win/
call makeTestbed.bat

echo(
echo(

rem if %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)
if %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && goto end)

echo All assemblies built successfully
goto end

:end
pause
exit