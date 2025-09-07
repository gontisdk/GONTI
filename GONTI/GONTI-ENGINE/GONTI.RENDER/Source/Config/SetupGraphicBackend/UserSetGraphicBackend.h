#ifndef USERSETGRAPHICBACKEND_H
#define USERSETGRAPHICBACKEND_H

#ifdef __cplusplus
    extern "C" {
#endif

        /* ======================================== [VULKAN] ======================================== */

        #define GONTI_USE_VULKAN 1
        #define GONTI_USE_VK GONTI_USE_VULKAN

        /* ======================================== [OPENGL] ======================================== */

        #define GONTI_USE_OPENGL 0
        #define GONTI_USE_GL GONTI_USE_OPENGL

        /* ======================================== [DIRECTX] ======================================== */

        #define GONTI_USE_DIRECTX_11 0
        #define GONTI_USE_DX11 GONTI_USE_DIRECTX_11

        #define GONTI_USE_DIRECTX_12 0
        #define GONTI_USE_DX12 GONTI_USE_DIRECTX_12

#ifdef __cplusplus
    }
#endif

#endif