#ifndef VULKANGAMETYPES_H
#define VULKANGAMETYPES_H

    #ifdef __cplusplus
        extern "C" {
    #endif

            #include "../../../__GONTI_INCLUDES.h"

            #ifdef USE_VULKAN
            #ifdef GameTypesVulkan_ON
                
                #include "../../../Environments/Vulkan/Application/VulkanApplication.h"

                typedef struct gontiVkEntry {
                    ApplicationConfig appConfig;

                    b8 (*initialize) (struct gontiVkEntry* entryInst);
                    b8 (*update) (struct gontiVkEntry* entryInst, f32 deltaTime);
                    b8 (*render) (struct gontiVkEntry* entryInst, f32 deltaTime);

                    void (*registerEvents)();
                    void (*unregisterEvents)();

                    void (*onResize) (struct gontiVkEntry* entryInst, u32 width, u32 height);
                    void* state;
                }gontiVkEntry;

            #endif
            #endif

    #ifdef __cplusplus
        }
    #endif

#endif