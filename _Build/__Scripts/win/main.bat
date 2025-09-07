@echo off
title Clang Compiler Console by OXlatorty
rem Build Everything

echo Build Everything...

rem call makeEngine.bat
cd __GONTI
call main.bat

echo(
echo(

rem if %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)
if %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && goto endErr)

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
if %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && goto endErr)

:success
echo All build successfully

:endErr
pause
exit