@echo off

call make_GONTI.CORE.bat

echo(
echo(

rem if %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)
if %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && goto end)

echo(
echo(

call make_GONTI.RENDER.bat

echo(
echo(

rem if %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)
if %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && goto end)

echo(
echo(

call make_GONTI.RUNTIME.bat

echo(
echo(

rem if %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)
if %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && goto end)

echo(
echo(

call make_GONTI.AUDIO.bat

echo(
echo(

rem if %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)
if %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && goto end)

echo(
echo(