rem Make script for Engine
@echo off
SetLocal EnableDelayedExpansion

rem Zbieranie plików .c (Z Utilities\Build do PhysicsEngineSource)
cd ../../../../../GONTI/GONTI-ENGINE/GONTI.AUDIO/Source
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

cd ../../../..

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


if not exist "_Build/" (
    mkdir "_Build/"
)

cd _Build/

rem A tak o aby już było na zapas
if not exist "Include/GONTI/GONTI-ENGINE/GONTI.AUDIO" (
    mkdir "Include/GONTI/GONTI-ENGINE/GONTI.AUDIO"
)

rem Upewnij się, że folder build istnieje
if not exist "win/bin/GONTI/GONTI-ENGINE" (
    mkdir "win/bin/GONTI/GONTI-ENGINE"
)

cd win/bin/GONTI/GONTI-ENGINE

if not exist "GONTI.AUDIO" (
    mkdir "GONTI.AUDIO"
)

cd GONTI.AUDIO

rem Ścieżki do bibliotek
set vcpkgIncludeDir=C:/Program Files (x86)/VulkanSDK/1.4.321.1/Include
set vcpkgLibDir=C:/Program Files (x86)/VulkanSDK/1.4.321.1/Lib

rem for opengl -lopengl32

rem Kompilacja
set assembly=GONTI.AUDIO
set compilerFlags=-g -shared -Wvarargs -Wall -Werror
set includeFlags=-I"GONTI" -I"%vcpkgIncludeDir%"
set linkerFlags=-L"%vcpkgLibDir%" -luser32 -lvulkan-1 
set defines=-D_DEBUG -DKEXPORT -D_CRT_SECURE_NO_WARNINGS

echo Building %assembly%...

echo(
echo(
echo ===========================================================================
echo ===     START OF: make_GONTI.AUDIO.bat Errors/Warnings/Informations     ===
echo ===========================================================================
echo(
echo(
echo(

clang !cFilenames! %compilerFlags% -o %assembly%.dll %defines% %includeFlags% %linkerFlags%

echo(
echo(
echo(
echo =========================================================================
echo ===     END OF: make_GONTI.AUDIO.bat Errors/Warnings/Informations     ===
echo =========================================================================
echo(
echo(
echo(