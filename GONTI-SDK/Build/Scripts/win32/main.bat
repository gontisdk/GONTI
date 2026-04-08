@echo off
echo "Building everything..."

set make="C:\Program Files (x86)\GnuWin32\bin\make.exe"

rem go to makefiles
cd GONTI/GONTI-ENGINE

rem Build GONTI.CORE
%make% -f "Makefile.GONTI.CORE.win32.mak" all
if %ERRORLEVEL% NEQ 0 (echo ERROR:%ERRORLEVEL% && goto errEnd)

rem Build GONTI.RENDER.VK
%make% -f "Makefile.GONTI.RENDER.VK.win32.mak" all
if %ERRORLEVEL% NEQ 0 (echo ERROR:%ERRORLEVEL% && goto errEnd)

rem Build GONTI.RENDER
%make% -f "Makefile.GONTI.RENDER.win32.mak" all
if %ERRORLEVEL% NEQ 0 (echo ERROR:%ERRORLEVEL% && goto errEnd)

rem Build GONTI.RUNTIME
%make% -f "Makefile.GONTI.RUNTIME.win32.mak" all
if %ERRORLEVEL% NEQ 0 (echo ERROR:%ERRORLEVEL% && goto errEnd)

:errEnd
pause
exit

:end
echo "All assemblies built successfully."