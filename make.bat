@echo off
echo "Building GONTI-SDK collections..."

rem ====================================================================================================================================================== #

echo "Building GONTI-CORE parts..."

rem Build GONTI.CORE
make -f "Build/Scripts/win32/GONTI-SDK/GONTI-CORE/Makefile.GONTI.CORE.win32.mak" all
if %ERRORLEVEL% NEQ 0 (echo ERROR:%ERRORLEVEL% && goto errEnd)

rem Build GONTI.CONTAINERS
make -f "Build/Scripts/win32/GONTI-SDK/GONTI-CORE/Makefile.GONTI.CONTAINERS.win32.mak" all
if %ERRORLEVEL% NEQ 0 (echo ERROR:%ERRORLEVEL% && goto errEnd)

rem Build GONTI.MATH
make -f "Build/Scripts/win32/GONTI-SDK/GONTI-CORE/Makefile.GONTI.MATH.win32.mak" all
if %ERRORLEVEL% NEQ 0 (echo ERROR:%ERRORLEVEL% && goto errEnd)

rem Build GONTI.STRING
make -f "Build/Scripts/win32/GONTI-SDK/GONTI-CORE/Makefile.GONTI.STRING.win32.mak" all
if %ERRORLEVEL% NEQ 0 (echo ERROR:%ERRORLEVEL% && goto errEnd)

rem Build GONTI.FILESYSTEM
make -f "Build/Scripts/win32/GONTI-SDK/GONTI-CORE/Makefile.GONTI.FILESYSTEM.win32.mak" all
if %ERRORLEVEL% NEQ 0 (echo ERROR:%ERRORLEVEL% && goto errEnd)

rem ====================================================================================================================================================== #

echo "Building GONTI-RENDER-VK parts..."

rem Build GONTI.RENDER.DEBUGGER.VK
make -f "Build/Scripts/win32/GONTI-SDK/GONTI-RENDER/GONTI-RENDER-VK/Makefile.GONTI.RENDER.DEBUGGER.VK.win32.mak" all
if %ERRORLEVEL% NEQ 0 (echo ERROR:%ERRORLEVEL% && goto errEnd)

rem Build GONTI.RENDER.UTILITIES.VK
make -f "Build/Scripts/win32/GONTI-SDK/GONTI-RENDER/GONTI-RENDER-VK/Makefile.GONTI.RENDER.UTILITIES.VK.win32.mak" all
if %ERRORLEVEL% NEQ 0 (echo ERROR:%ERRORLEVEL% && goto errEnd)

rem Build GONTI.RENDER.CORE.VK
make -f "Build/Scripts/win32/GONTI-SDK/GONTI-RENDER/GONTI-RENDER-VK/Makefile.GONTI.RENDER.CORE.VK.win32.mak" all
if %ERRORLEVEL% NEQ 0 (echo ERROR:%ERRORLEVEL% && goto errEnd)

rem Build GONTI.RENDER.SHADERS.VK
make -f "Build/Scripts/win32/GONTI-SDK/GONTI-RENDER/GONTI-RENDER-VK/Makefile.GONTI.RENDER.SHADERS.VK.win32.mak" all
if %ERRORLEVEL% NEQ 0 (echo ERROR:%ERRORLEVEL% && goto errEnd)

rem Build GONTI.RENDER.VK
make -f "Build/Scripts/win32/GONTI-SDK/GONTI-RENDER/GONTI-RENDER-VK/Makefile.GONTI.RENDER.VK.win32.mak" all
if %ERRORLEVEL% NEQ 0 (echo ERROR:%ERRORLEVEL% && goto errEnd)

rem ====================================================================================================================================================== #

echo "Building GONTI-RENDER parts..."

rem Build GONTI.RENDER
make -f "Build/Scripts/win32/GONTI-SDK/GONTI-RENDER/Makefile.GONTI.RENDER.win32.mak" all
if %ERRORLEVEL% NEQ 0 (echo ERROR:%ERRORLEVEL% && goto errEnd)

rem ====================================================================================================================================================== #

echo "Building GONTI-RUNTIME parts..."

rem Build GONTI.RUNTIME
make -f "Build/Scripts/win32/GONTI-SDK/GONTI-RUNTIME/Makefile.GONTI.RUNTIME.win32.mak" all
if %ERRORLEVEL% NEQ 0 (echo ERROR:%ERRORLEVEL% && goto errEnd)

goto end

:errEnd
pause
exit

:end
echo "---------------------------------------"
echo "All assemblies built successfully."