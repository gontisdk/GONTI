#ifndef __ENABLEDS_H
#define __ENABLEDS_H

    // GONTI/
    #define GONTI_ON
    #ifdef GONTI_ON

        #define GONTI_Includes_ON
        #define Validation_ON

        // GONTI/Math/
		#define Maths_ON
        #ifdef Maths_ON

            // GONTI/Math/Vectors/
            #define Vectors_ON

            // GONTI/Math/Matrices/
		    #define Matrices_ON

            // GONTI/Math/Algebra/
            #define Algebra_ON
            #ifdef Algebra_ON

                // GONTI/Math/Algebra/BigInteger/
                #define BigInteger_ON

            #endif // !GONTI/Math/Algebra/

        #endif // !GONTI/Math/

        // GONTI/Utilities/Logger/Scripts/
		#define Logger_ON
        #ifdef Logger_ON
        
        #endif // !GONTI/Utilities/Logger/Scripts/

        // GONTI/Core/
        #define Core_ON
        #ifdef Core_ON

            // GONTI/Core/Platform/
            #define Platform_ON
            #ifdef Platform_ON

                // GONTI/Core/Platform/Vulkan/
                #define PlatformVulkan_ON

                // GONTI/Core/Platform/OpenGL/
                #define PlatformOpenGL_ON

            #endif // !GONTI/Core/Platform/

            // GONTI/Core/CoreEntry/
            #define CoreEntry_ON
            #ifdef CoreEntry_ON

                // GONTI/Core/CoreEntry/Vulkan/
                #define EntryPointVulkan_ON

                // GONTI/Core/CoreEntry/OpenGL/
                #define EntryPointOpenGL_ON

            #endif // !GONTI/Core/CoreEntry/

            // GONTI/Core/GameTypes/
            #define GameTypes_ON
            #ifdef GameTypes_ON

                // GONTI/Core/GameTypes/Vulkan/
                #define GameTypesVulkan_ON

                // // GONTI/Core/GameTypes/OpenGL/
                #define GameTypesOpenGL_ON

            #endif // !GONTI/Core/GameTypes/

            // GONTI/Core/MemorySubsystem/
            #define MemorySubsystem_ON
            #ifdef MemorySubsystem_ON

                

            #endif // !GONTI/Core/MemorySubsystem/

        #endif // !GONTI/Core

        // GONTI/Environments/
        #define Environments_ON
        #ifdef Environments_ON

            // GONTI/Environments/Vulkan/Application
            #define ApplicationVulkan_ON

            // GONTI/Environments/OpenGL/Application
            #define ApplicationOpenGL_ON

        #endif // !GONTI/Environments/

        // GONTI/Events/
        #define Events_ON
        #ifdef Events_ON

            #define EventsVulkan_ON
            #define EventsOpenGL_ON

            // GONTI/Events/InputSystem/
            #define InputSystem_ON
            #ifdef InputSystem_ON

                // GONTI/Events/InputSystem/Vulkan/
                #define InputSystemVulkan_ON

                // GONTI/Events/InputSystem/OpenGL/
                #define InputSystemOpenGL_ON

            #endif

        #endif

        #define Containers_ON
        #ifdef Containers_ON

            #define DynamicArray_ON
            #define DynamicArrayEx_ON

        #endif

        #define StringLib_ON
        #ifdef StringLib_ON
        
        #endif

        #define RendererSubsystem_ON
        #ifdef RendererSubsystem_ON

            #define DeltaTime_ON
            #define RendererBackend_ON
            #define RendererFrontend_ON
            #define Swapchain_ON
            #define Image_ON
            #define Renderpass_ON
            #define CommandBuffer_ON
            #define FrameBuffer_ON
            #define SyncObjects_ON
            #define Fence_ON

        #endif

    #endif // !GONTI/

#endif