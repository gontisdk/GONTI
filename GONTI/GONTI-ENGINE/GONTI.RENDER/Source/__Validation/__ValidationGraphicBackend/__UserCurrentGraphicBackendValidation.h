#ifndef __USERCURRENTGRAPHICBACKENDVALIDATION_H
#define __USERCURRENTGRAPHICBACKENDVALIDATION_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../../Config/SetupGraphicBackend/UserSetGraphicBackend.h"

        #if (GONTI_USE_VULKAN + GONTI_USE_VK) >= 1 && (GONTI_USE_OPENGL + GONTI_USE_GL) >= 1 && ((GONTI_USE_DIRECTX_11 + GONTI_USE_DX11) >= 1 || (GONTI_USE_DIRECTX_12 + GONTI_USE_DX12) >= 1) >= 1
            #error You define all graphic backends Vulkan and OpenGL and DirectX, please pick one
        #elif (GONTI_USE_VULKAN + GONTI_USE_VK) >= 1 && (GONTI_USE_OPENGL + GONTI_USE_GL) >= 1
            #error You define two graphic backends Vulkan nad OpenGL, please pick one!
        #elif (GONTI_USE_VULKAN + GONTI_USE_VK) >= 1 && ((GONTI_USE_DIRECTX_11 + GONTI_USE_DX11) >= 1 || (GONTI_USE_DIRECTX_12 + GONTI_USE_DX12) >= 1) >= 1
            #error You define two graphic backends Vulkan nad DirectX, please pick one!
        #elif (GONTI_USE_OPENGL + GONTI_USE_GL) >= 1 && ((GONTI_USE_DIRECTX_11 + GONTI_USE_DX11) >= 1 || (GONTI_USE_DIRECTX_12 + GONTI_USE_DX12) >= 1) >= 1
            #error You define two graphic backends OpenGL nad DirectX, please pick one!
        #elif (GONTI_USE_VULKAN + GONTI_USE_VK) + (GONTI_USE_OPENGL + GONTI_USE_GL) + ((GONTI_USE_DIRECTX_11 + GONTI_USE_DX11) + (GONTI_USE_DIRECTX_12 + GONTI_USE_DX12)) == 0
            #error Graphic backend not defined.
        #endif

#ifdef __cplusplus
    }
#endif

#endif