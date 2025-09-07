#ifndef __GRAPHICSBOOTSTRAP_H
#define __GRAPHICSBOOTSTRAP_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../../Config/Defaults/__ENABLEDS.h"

        #if !defined(__ENABLEDS_H)
            #error "Original file ENABLEDS.h not found, propably your file is not oryginal or has been edited more than you need"
        #endif

        #ifdef Validation_ON

            #include "../../Config/Defaults/__EngineUserConfig.h"
            
            // Validation
            #if !defined(__ENGINE_USER_CONFIG_H)
                #error "Original file EngineUserConfig.h not found, propably your file is not oryginal or has been edited more than you need"
            #elif defined(USE_VULKAN) && defined(USE_OPENGL) && defined(USE_DIRECTX)
                #error "Cannot define all. Pick only one."
            #elif defined(USE_VULKAN) && defined(USE_OPENGL)
                #error "Cannot define both USE_VULKAN and USE_OPENGL. Pick only one."
            #elif defined(USE_VULKAN) && defined(USE_DIRECTX)
                #error "Cannot define both USE_VULKAN and USE_DIRECTX. Pick only one."
            #elif defined(USE_OPENGL) && defined(USE_DIRECTX)
                #error "Cannot define both USE_OPENGL and USE_DIRECTX. Pick only one."
            #elif !defined(USE_VULKAN) && !defined(USE_OPENGL) && !defined(USE_DIRECTX)
                #error "No graphic environment defined. Please define either: USE_VULKAN or USE_OPENGL or USE_DIRECTX"
            #endif

            #if defined(USE_OPENGL)
                #if defined(OPENGL_GLAD) && defined(OPENGL_GLEW)
                    #error "Cannot define both OpenGL versions, plase define one"
                #endif
                #if !defined(OPENGL_GLAD) && !defined(OPENGL_GLEW)
                    #error "No OpenGL version defined. Plase define either: OPENGL_GLAD or OPENGL_GLEW"
                #endif
            #endif

            #if defined(USE_DIRECTX)
                #if defined(DIRECTX_X11) && defined(DIRECTX_X12)
                    #error "Cannot define both DirectX versions, plase define one"
                #endif
                #if !defined(DIRECTX_X11) && !defined(DIRECTX_X12)
                    #error "No DirectX version defined. Plase define either: DIRECTX_X11 or DIRECTX_X12"
                #endif
            #endif
        #endif

#ifdef __cplusplus
    }
#endif

#endif