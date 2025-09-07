#ifndef VULKANENTRYTYPES_INL
#define VULKANENTRYTYPES_INL

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/Defines.h>

        typedef struct GontiVulkanEntryWindowConfig {
            char* windowName;
            char* className;

            i16 startPosX;
            i16 startPosY;
            i16 startWidth;
            i16 startHeight;
        } GontiVulkanEntryWindowConfig;

        typedef struct GontiVulkanEntry {
            GontiVulkanEntryWindowConfig windowConfig;

            b8 (*initialize) (struct GontiVulkanEntry* entryInst);
            b8 (*update) (struct GontiVulkanEntry* entryInst, f32 deltaTime);
            b8 (*render) (struct GontiVulkanEntry* entryInst, f32 deltaTime);

            void (*onResize) (struct GontiVulkanEntry* entryInst, u32 width, u32 height);
            void (*shutdown) (struct GontiVulkanEntry* entryInst);
            void* state;
        } GontiVulkanEntry;

#ifdef __cplusplus
    }
#endif

#endif