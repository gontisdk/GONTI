#ifndef GtVkBufferTypesINL
#define GtVkBufferTypesINL

#ifdef __cplusplus
    extern "C" {
#endif

        #include <vulkan/vulkan.h>
        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.h>

        typedef struct GtVkBuffer {
            GtU64 totalSize;
            VkBuffer handle;
            VkBufferUsageFlagBits usage;
            GtB8 isLocked;
            VkDeviceMemory memory;
            GtI32 memoryIndex;
            GtU32 memoryPropertyFlags;
        } GtVkBuffer;

#ifdef __cplusplus
    }
#endif

#endif