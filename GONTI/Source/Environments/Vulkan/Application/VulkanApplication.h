#ifndef VULKANAPPLICATION_H
#define VULKANAPPLICATION_H

    #ifdef __cplusplus
    extern "C" {
    #endif

        #include "../../../__GONTI_INCLUDES.h"

        #ifdef USE_VULKAN
        #ifdef ApplicationVulkan_ON

            struct gontiVkEntry;

            typedef struct ApplicationConfig {
                char* appName;
                char* className;

                i16 startPosX;
                i16 startPosY;
                i16 startWidth;
                i16 startHeight;
            }ApplicationConfig;
            
            /* BOOL */
            KAPI b8 applicationCreate(struct gontiVkEntry* entryInst);
            KAPI b8 applicationRun(u64 fpsLimit, b8 isFpsLimit);

            /* VOID */
            KAPI void applicationShutDown();
            KAPI void applicationPause();
            KAPI void applicationResume();
            KAPI void applicationGetFramebufferSize(u32* width, u32* height);

        #endif
        #endif

    #ifdef __cplusplus 
    }
    #endif

#endif