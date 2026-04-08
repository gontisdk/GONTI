#!/bin/bash

# -e: przerywa skrypt przy błędzie którejkolwiek komendy
set -e

echo "Building everything..."

# Na Linuxie 'make' zazwyczaj jest w PATH, nie trzeba podawać pełnej ścieżki
MAKE="make"

# Przejdź do folderu z Makefile'ami
cd GONTI/GONTI-ENGINE

# Build GONTI.CORE
echo "Building GONTI.CORE..."
$MAKE -f "Makefile.GONTI.CORE.linux.mak" all

# Build GONTI.RENDER.VK
echo "Building GONTI.RENDER.VK..."
$MAKE -f "Makefile.GONTI.RENDER.VK.linux.mak" all

# Build GONTI.RENDER
echo "Building GONTI.RENDER..."
$MAKE -f "Makefile.GONTI.RENDER.linux.mak" all

# Build GONTI.RUNTIME
echo "Building GONTI.RUNTIME..."
$MAKE -f "Makefile.GONTI.RUNTIME.linux.mak" all

echo "---------------------------------------"
echo "All assemblies built successfully."

# Odpowiednik 'pause' z Windowsa (opcjonalne w terminalu)
#read -p "Press [Enter] key to continue..."