@echo off
echo "Building everything..."

rem Build GONTI.CORE
make -f "Build/Scripts/win32/GONTI/GONTI-ENGINE/Makefile.GONTI.CORE.win32.mak" all
if %ERRORLEVEL% NEQ 0 (echo ERROR:%ERRORLEVEL% && goto errEnd)

rem Build GONTI.RENDER.VK
make -f "Build/Scripts/win32/GONTI/GONTI-ENGINE/Makefile.GONTI.RENDER.VK.win32.mak" all
if %ERRORLEVEL% NEQ 0 (echo ERROR:%ERRORLEVEL% && goto errEnd)

rem Build GONTI.RENDER
make -f "Build/Scripts/win32/GONTI/GONTI-ENGINE/Makefile.GONTI.RENDER.win32.mak" all
if %ERRORLEVEL% NEQ 0 (echo ERROR:%ERRORLEVEL% && goto errEnd)

rem Build GONTI.RUNTIME
make -f "Build/Scripts/win32/GONTI/GONTI-ENGINE/Makefile.GONTI.RUNTIME.win32.mak" all
if %ERRORLEVEL% NEQ 0 (echo ERROR:%ERRORLEVEL% && goto errEnd)

:errEnd
pause
exit

:end
echo "---------------------------------------"
echo "All assemblies built successfully."