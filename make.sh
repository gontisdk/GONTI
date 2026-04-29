#!/bin/bash
set -e
echo "Building GONTI-ENGINE parts..."

echo "Building GONTI.CORE..."
make -f "Build/Scripts/linux/GONTI/GONTI-ENGINE/Makefile.GONTI.CORE.linux.mak" all

echo "Building GONTI.RENDER.VK..."
make -f "Build/Scripts/linux/GONTI/GONTI-ENGINE/Makefile.GONTI.RENDER.VK.linux.mak" all

echo "Building GONTI.RENDER..."
make -f "Build/Scripts/linux/GONTI/GONTI-ENGINE/Makefile.GONTI.RENDER.linux.mak" all

echo "Building GONTI.RUNTIME..."
make -f "Build/Scripts/linux/GONTI/GONTI-ENGINE/Makefile.GONTI.RUNTIME.linux.mak" all

echo "---------------------------------------"
echo "All assemblies built successfully."