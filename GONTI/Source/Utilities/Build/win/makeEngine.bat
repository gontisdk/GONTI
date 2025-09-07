rem Make script for Engine
@echo off
SetLocal EnableDelayedExpansion

rem Zbieranie plików .c (Z Utilities\Build do PhysicsEngineSource)
cd ../../../../
set cFilenames=

rem for /R %%f in (*.c) do (
rem     set "cFilenames=!cFilenames! "%%f""
rem )

for /R %%f in (*.c) do (
    set "filepath=%%f"
    rem Sprawdź czy w ścieżce jest \Tests\
    echo !filepath! | findstr /I /C:"\\Tests\\" >nul
    if errorlevel 1 (
        rem Jeśli nie ma \Tests\, sprawdź czy ścieżka nie kończy się na \Tests
        echo !filepath! | findstr /I /E /C:"\\Tests" >nul
        if errorlevel 1 (
            rem Nie znaleziono \Tests\ ani na końcu \Tests - dodaj plik
            set "cFilenames=!cFilenames! "%%f""
        )
    )
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

cd ..

if not exist "_Build/" (
    mkdir "_Build/"
)

cd _Build/

rem A tak o aby już było na zapas
    if not exist "Include/GONTI" (
        mkdir "Include/GONTI"
    )

rem Upewnij się, że folder build istnieje
if not exist "win/bin/GONTI" (
    mkdir "win/bin/GONTI"
)

cd win/bin/GONTI

rem Ścieżki do bibliotek
set vcpkgIncludeDir=D:/Programs Files/vcpkg/vcpkg/installed/x64-windows/include
set vcpkgLibDir=D:/Programs Files/vcpkg/vcpkg/installed/x64-windows/lib

rem NOTE: Używamy Vulkan z vcpkg na ten moment
rem set IncludeDirVulkan=D:/Programs Files/VulkanSDK/1.4.313.2/Include
rem set LibDirVulkan=D:/Programs Files/VulkanSDK/1.4.313.2/Lib

rem for opengl -lopengl32

rem Kompilacja
set assembly=GONTI
set compilerFlags=-g -shared -Wvarargs -Wall -Werror
set includeFlags=-I"GONTI" -I"%vcpkgIncludeDir%"
set linkerFlags=-L"%vcpkgLibDir%" -luser32 -lvulkan-1 
set defines=-D_DEBUG -DKEXPORT -D_CRT_SECURE_NO_WARNINGS

echo Building %assembly%...

echo(
echo(
echo =====================================================================
echo ===     START OF: MakeEngine.bat Errors/Warnings/Informations     ===
echo =====================================================================
echo(
echo(
echo(

clang !cFilenames! %compilerFlags% -o %assembly%.dll %defines% %includeFlags% %linkerFlags%

echo(
echo(
echo(
echo ===================================================================
echo ===     END OF: MakeEngine.bat Errors/Warnings/Informations     ===
echo ===================================================================