rem Make script for testbed
@echo off
SetLocal EnableDelayedExpansion

rem Przejdź do katalogu źródłowego
rem cd .. rem Wyjście z   GONTI/Source/Utilities/Build/win/   do     GONTI/Source/Utilities/Build/
rem cd .. rem Wyjście z   GONTI/Source/Utilities/Build/       do     GONTI/Source/Utilities/
rem cd .. rem Wyjście z   GONTI/Source/Utilities/             do     GONTI/Source/
rem cd .. rem Wyjście z   GONTI/Source/                       do     GONTI/

rem Wyczyść zmienną
set cFilenames=

rem Dodaj wszystkie pliki .c z Tests/
for /R ..\..\..\..\Tests %%f in (*.c) do (
    set "cFilenames=!cFilenames! "%%f""
)

echo(
echo ===============================================
echo === START LIST OF: Files passed to compiler ===
echo ===============================================
echo(
echo !cFilenames!
echo(
echo =============================================
echo === END LIST OF: Files passed to compiler ===
echo =============================================
echo(
echo(


if not exist "..\..\..\..\_Build/" (
    mkdir "..\..\..\..\_Build/"
)

cd ..\..\..\..\_Build

rem Upewnij się, że folder build istnieje
if not exist "win/testbed" (
    mkdir "win/testbed"
)

cd win/testbed

rem Ścieżki do bibliotek
<<<<<<< HEAD
set vcpkgIncludeDir=C:/Program Files (x86)/VulkanSDK/1.4.321.1/Include
set vcpkgLibDir=C:/Program Files (x86)/VulkanSDK/1.4.321.1/Lib

set workspaceFoldersInclude=D:\Projects\Visual Studio Code Projects\C++\GONTI SDK
set GONTI-CORE-Libs-Dir=D:\Projects\Visual Studio Code Projects\C++\GONTI SDK\_Build\win\bin\GONTI\GONTI-ENGINE\GONTI.CORE
set GONTI-RENDER-Libs-Dir=D:\Projects\Visual Studio Code Projects\C++\GONTI SDK\_Build\win\bin\GONTI\GONTI-ENGINE\GONTI.RENDER
set GONTI-RUNTIME-Libs-Dir=D:\Projects\Visual Studio Code Projects\C++\GONTI SDK\_Build\win\bin\GONTI\GONTI-ENGINE\GONTI.RUNTIME
=======
set vcpkgIncludeDir=D:/Programs Files/vcpkg/vcpkg/installed/x64-windows/include
set vcpkgLibDir=D:/Programs Files/vcpkg/vcpkg/installed/x64-windows/lib

set IncludeDirGLFW=D:/Dokumenty/My Liblares to C++/glfw-3.4.bin.WIN64/include
set LibDirGLFW=D:/Dokumenty/My Liblares to C++/glfw-3.4.bin.WIN64/lib-vc2022
>>>>>>> 475c679c815ea5f049414def8adf10b6b1ed375a

rem Kompilacja
set assembly=testbed
set compilerFlags=-g
<<<<<<< HEAD
set includeFlags=-I"%workspaceFoldersInclude%" -I"%vcpkgIncludeDir%"
set linkerFlags=-L"..\bin" -L"%GONTI-CORE-Libs-Dir%" -L"%GONTI-RENDER-Libs-Dir%", -L"%GONTI-RUNTIME-Libs-Dir%" -L"%vcpkgLibDir%" -luser32 -lGONTI.CORE -lGONTI.RENDER -lGONTI.RUNTIME -lvulkan-1
=======
set includeFlags=-I"GONTI" -I"%vcpkgIncludeDir%" -I"%IncludeDirGLFW%"
set linkerFlags=-L"..\bin" -lGONTI/GONTI -L"%vcpkgLibDir%" -L"%LibDirGLFW%" -luser32 -lglfw3
>>>>>>> 475c679c815ea5f049414def8adf10b6b1ed375a
set defines=-D_DEBUG -DKIMPORT

echo Building %assembly%...

<<<<<<< HEAD
echo(
echo(

echo Copying GONTI.CORE.dll to testbed directory...
copy "..\bin\GONTI\GONTI-ENGINE\GONTI.CORE\GONTI.CORE.dll" ".\GONTI.CORE.dll" /Y > nul
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to copy GONTI.CORE.dll!
) else (
    echo [SUCCESS] GONTI.CORE.dll copied successfully.
)

echo Copying GONTI.RENDER.dll to testbed directory...
copy "..\bin\GONTI\GONTI-ENGINE\GONTI.RENDER\GONTI.RENDER.dll" ".\GONTI.RENDER.dll" /Y > nul
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to copy GONTI.RENDER.dll!
) else (
    echo [SUCCESS] GONTI.RENDER.dll copied successfully.
)

echo Copying GONTI.RUNTIME.dll to testbed directory...
copy "..\bin\GONTI\GONTI-ENGINE\GONTI.RUNTIME\GONTI.RUNTIME.dll" ".\GONTI.RUNTIME.dll" /Y > nul
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to copy GONTI.RUNTIME.dll!
) else (
    echo [SUCCESS] GONTI.RUNTIME.dll copied successfully.
)

echo(
echo(

=======
echo Copying GONTI.dll to testbed directory...
copy "..\bin\GONTI\GONTI.dll" ".\GONTI.dll" /Y > nul
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to copy GONTI.dll!
) else (
    echo GONTI.dll copied successfully.
)

>>>>>>> 475c679c815ea5f049414def8adf10b6b1ed375a
echo(
echo(
echo ======================================================================
echo ===     START OF: MakeTestbed.bat Errors/Warnings/Informations     ===
echo ======================================================================
echo(
echo(
echo(

rem  "../../../Tests/main.c" = path to your main file (is in Tests/ folder so it is no needed)
clang !cFilenames! %compilerFlags% -o %assembly%.exe %defines% %includeFlags% %linkerFlags%

echo(
echo(
echo(
echo ====================================================================
echo ===     END OF: MakeTestbed.bat Errors/Warnings/Informations     ===
echo ====================================================================

rem call run.bat
call "%~dp0run.bat" %assembly%

goto end
:end