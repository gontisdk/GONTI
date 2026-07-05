#ifndef GtVkBuffersH
#define GtVkBuffersH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI-CORE/GONTI.CORE/Source/Defines/GtDefines.inl>
        #include <vulkan/vulkan.h>
        #include <Resources/Types/GtVkBufferTypes.inl>
        #include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.COMMON.VK/Source/Types/GtVkContextTypes.inl>

        /* B8 */
        GTAPI GtB8 gontiVkBuffersCreate(GtVkContext* context);
        GTAPI GtB8 gontiVkBufferCreate(
            GtVkContext* context,
            GtU64 size,
            VkBufferUsageFlagBits usage,
            GtU32 memoryPropertyFlags,
            GtB8 bindOnCreate,
            GtVkBuffer* outBuffer
        );
        GTAPI GtB8 gontiVkBufferResize(
            GtVkContext* context,
            GtU64 newSize,
            GtVkBuffer* buffer,
            VkQueue queue,
            VkCommandPool pool
        );

        /* VOID */
        GTAPI void gontiVkBuffersDestroy(GtVkContext* context);
        GTAPI void gontiVkBufferDestroy(GtVkContext* context, GtVkBuffer* buffer);
        GTAPI void gontiVkBufferUnlockMemory(GtVkContext* context, GtVkBuffer* buffer);
        GTAPI void gontiVkBufferLoadData(GtVkContext* context, GtVkBuffer* buffer, GtU64 offset, GtU64 size, GtU32 flags, const void* data);
        GTAPI void gontiVkBufferBind(GtVkContext* context, GtVkBuffer* buffer, GtU64 offset);
        GTAPI void gontiVkBufferCopyTo(
            GtVkContext* context,
            VkCommandPool pool,
            VkFence fence,
            VkQueue queue,
            VkBuffer source,
            GtU64 sourceOffset,
            VkBuffer dest,
            GtU64 destOffset,
            GtU64 size
        );

        /* VOID* */
        GTAPI void* gontiVkBufferLockMemory(GtVkContext* context, GtVkBuffer* buffer, GtU64 offset, GtU64 size, GtU32 flags);

#ifdef __cplusplus
    }
#endif

#endif