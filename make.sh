#!/bin/bash
set -e
echo "Building GONTI-SDK collections..."

# ====================================================================================================================================================== #

echo "Building GONTI-CORE parts..."

echo
echo "Building GONTI.so..."
make -f "Build/Scripts/linux/GONTI-SDK/GONTI-CORE/Makefile.GONTI.CORE.linux.mak" all

echo
echo "Building GONTI.CONTAINERS.so..."
make -f "Build/Scripts/linux/GONTI-SDK/GONTI-CORE/Makefile.GONTI.CONTAINERS.linux.mak" all

echo
echo "Building GONTI.MATH.so..."
make -f "Build/Scripts/linux/GONTI-SDK/GONTI-CORE/Makefile.GONTI.MATH.linux.mak" all

echo 
echo "Building GONTI.STRING.so..."
make -f "Build/Scripts/linux/GONTI-SDK/GONTI-CORE/Makefile.GONTI.STRING.linux.mak" all

echo
echo "Building GONTI.FILESYSTEM.so..."
make -f "Build/Scripts/linux/GONTI-SDK/GONTI-CORE/Makefile.GONTI.FILESYSTEM.linux.mak" all


# ====================================================================================================================================================== #

echo
echo
echo

# ====================================================================================================================================================== #

echo "Building GONTI-RENDER-VK parts.."

echo
echo "Building GONTI.RENDER.DEBUGGER.VK"
make -f "Build/Scripts/linux/GONTI-SDK/GONTI-RENDER/GONTI-RENDER-VK/Makefile.GONTI.RENDER.DEBUGGER.VK.linux.mak" all

echo
echo "Building GONTI.RENDER.UTILITIES.VK"
make -f "Build/Scripts/linux/GONTI-SDK/GONTI-RENDER/GONTI-RENDER-VK/Makefile.GONTI.RENDER.UTILITIES.VK.linux.mak" all

echo
echo "Building GONTI.CORE.VK"
make -f "Build/Scripts/linux/GONTI-SDK/GONTI-RENDER/GONTI-RENDER-VK/Makefile.GONTI.RENDER.CORE.VK.linux.mak" all

echo
echo "Building GONTI.SHADERS.VK"
make -f "Build/Scripts/linux/GONTI-SDK/GONTI-RENDER/GONTI-RENDER-VK/Makefile.GONTI.RENDER.SHADERS.VK.linux.mak" all

echo
echo "Building GONTI.VK"
make -f "Build/Scripts/linux/GONTI-SDK/GONTI-RENDER/GONTI-RENDER-VK/Makefile.GONTI.RENDER.VK.linux.mak" all


# ====================================================================================================================================================== #

echo
echo
echo

# ====================================================================================================================================================== #

echo "Building GONTI-RENDER parts..."

echo
echo "Building GONTI.VK"
make -f "Build/Scripts/linux/GONTI-SDK/GONTI-RENDER/Makefile.GONTI.RENDER.linux.mak" all


# ====================================================================================================================================================== #

echo
echo
echo

# ====================================================================================================================================================== #


echo "Building GONTI-RUNTIME parts..."

echo
echo "Building GONTI.RUNTIME..."
make -f "Build/Scripts/linux/GONTI-SDK/GONTI-RUNTIME/Makefile.GONTI.RUNTIME.linux.mak" all

echo "---------------------------------------"
echo "All assemblies built successfully."