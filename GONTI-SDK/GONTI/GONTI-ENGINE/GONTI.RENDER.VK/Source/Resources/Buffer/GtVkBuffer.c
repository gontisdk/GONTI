#include "GtVkBuffer.h"

#include "../../Hardware/Device/GtVkDevice.h"
#include "../../Hardware/LogicalDevice/GtVkLDevice.h"
#include "../../Hardware/PhysicalDevice/GtVkPDevice.h"
#include "../../Subsystems/CommandBuffer/GtVkCmdBuffer.h"
#include "../../Util/GtVkUtil.h"
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/GtLogger.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Memory/GtMemory.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Math/Geometry/Vertices/GtVertexTypes.inl>

/*
* B8
*/
GtB8 gontiVkBuffersCreate(GtVkContext* context) {
    VkMemoryPropertyFlags memoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    const GtU64 vertexBufferSize = sizeof(GtVertex3D) * 1024 * 1024;
    if (!gontiVkBufferCreate(
        context,
        vertexBufferSize,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        memoryPropertyFlags,
        GtTrue,
        &context->objectVertexBuffer
    )) {GTERROR("Error creating vertex buffer."); return GtFalse;}
    context->geometryVertexOffset = 0;

    const GtU64 indexBufferSize = sizeof(GtU32) * 1024 * 1024;
    if (!gontiVkBufferCreate(
        context,
        indexBufferSize,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        memoryPropertyFlags,
        GtTrue,
        &context->objectIndexBuffer
    )) {GTERROR("Error creating index buffer."); return GtFalse;}
    context->geometryIndexOffset = 0;

    return GtTrue;
}
GtB8 gontiVkBufferCreate(
    GtVkContext* context,
    GtU64 size,
    VkBufferUsageFlagBits usage,
    GtU32 memoryPropertyFlags,
    GtB8 bindOnCreate,
    GtVkBuffer* outBuffer
) {
    gt_zeroMemory(outBuffer, sizeof(GtVkBuffer));
    outBuffer->totalSize = size;
    outBuffer->usage = usage;
    outBuffer->memoryPropertyFlags = memoryPropertyFlags;

    VkBufferCreateInfo bufferInfo = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; // NOTE: Only used is one queue

    GTVK_CHECK(vkCreateBuffer(context->device.logicalDevice, &bufferInfo, context->allocator, &outBuffer->handle));

    VkMemoryRequirements requirements;
    vkGetBufferMemoryRequirements(context->device.logicalDevice, outBuffer->handle, &requirements);
    outBuffer->memoryIndex = context->gontiVkFindMemoryIndex(requirements.memoryTypeBits, outBuffer->memoryPropertyFlags);
    if (outBuffer->memoryIndex == -1) {
        GTERROR("Unable to create vulkan buffer beacuse required memory type index was not found.");
        return GtFalse;
    }

    VkMemoryAllocateInfo allocateInfo = {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    allocateInfo.allocationSize = requirements.size;
    allocateInfo.memoryTypeIndex = (GtU32)outBuffer->memoryIndex;

    VkResult result = vkAllocateMemory(
        context->device.logicalDevice,
        &allocateInfo,
        context->allocator,
        &outBuffer->memory
    );
    if (result != VK_SUCCESS) {
        GTERROR("Unable to create vulkan buffer because the required memory allocation failed. Error: %i", result);
        return GtFalse;
    }

    if (bindOnCreate) gontiVkBufferBind(context, outBuffer, 0);
    return GtTrue;
}
GtB8 gontiVkBufferResize(
    GtVkContext* context,
    GtU64 newSize,
    GtVkBuffer* buffer,
    VkQueue queue,
    VkCommandPool pool
) {
    VkBufferCreateInfo bufferInfo = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    bufferInfo.size = newSize;
    bufferInfo.usage = buffer->usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; // NOTE: Only used is one queue

    VkBuffer newBuffer;
    GTVK_CHECK(vkCreateBuffer(context->device.logicalDevice, &bufferInfo, context->allocator, &newBuffer));

    VkMemoryRequirements requirements;
    vkGetBufferMemoryRequirements(context->device.logicalDevice, newBuffer, &requirements);

    VkMemoryAllocateInfo allocateInfo = {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    allocateInfo.allocationSize = requirements.size;
    allocateInfo.memoryTypeIndex = (GtU32)buffer->memoryIndex;

    VkDeviceMemory newMemory;
    VkResult result = vkAllocateMemory(
        context->device.logicalDevice,
        &allocateInfo,
        context->allocator,
        &newMemory
    );
    if (result != VK_SUCCESS) {
        GTERROR("Unable to resize vulkan buffer because the required memory allocator failed. Error: %i", result);
        return GtFalse;
    }

    GTVK_CHECK(vkBindBufferMemory(context->device.logicalDevice, newBuffer, newMemory, 0));
    gontiVkBufferCopyTo(context, pool, 0, queue, buffer->handle, 0, newBuffer, 0, buffer->totalSize);
    vkDeviceWaitIdle(context->device.logicalDevice);

    if (buffer->memory) {
        vkFreeMemory(context->device.logicalDevice, buffer->memory, context->allocator);
        buffer->memory = 0;
    }

    if (buffer->handle) {
        vkDestroyBuffer(context->device.logicalDevice, buffer->handle, context->allocator);
        buffer->handle = 0;
    }

    buffer->totalSize = newSize;
    buffer->memory = newMemory;
    buffer->handle = newBuffer;

    return GtTrue;
}

/* 
* VOID 
*/
void gontiVkBuffersDestroy(GtVkContext* context) {
    gontiVkBufferDestroy(context, &context->objectVertexBuffer);
    gontiVkBufferDestroy(context, &context->objectIndexBuffer);
}
void gontiVkBufferDestroy(GtVkContext* context, GtVkBuffer* buffer) {
    if (buffer->memory) {
        vkFreeMemory(context->device.logicalDevice, buffer->memory, context->allocator);
        buffer->memory = 0;
    }

    if (buffer->handle) {
        vkDestroyBuffer(context->device.logicalDevice, buffer->handle, context->allocator);
        buffer->handle = 0;
    }

    buffer->totalSize = 0;
    buffer->usage = 0;
    buffer->isLocked = GtFalse;
}
void gontiVkBufferUnlockMemory(GtVkContext* context, GtVkBuffer* buffer) {
    vkUnmapMemory(context->device.logicalDevice, buffer->memory);
}
void gontiVkBufferLoadData(GtVkContext* context, GtVkBuffer* buffer, GtU64 offset, GtU64 size, GtU32 flags, const void* data) {
    void* dataPtr;
    GTVK_CHECK(vkMapMemory(context->device.logicalDevice, buffer->memory, offset, size, flags, &dataPtr));
    gt_copyMemory(dataPtr, data, size);
    vkUnmapMemory(context->device.logicalDevice, buffer->memory);
}
void gontiVkBufferBind(GtVkContext* context, GtVkBuffer* buffer, GtU64 offset) {
    GTVK_CHECK(vkBindBufferMemory(context->device.logicalDevice, buffer->handle, buffer->memory, offset));
}
void gontiVkBufferCopyTo(
    GtVkContext* context,
    VkCommandPool pool,
    VkFence fence,
    VkQueue queue,
    VkBuffer source,
    GtU64 sourceOffset,
    VkBuffer dest,
    GtU64 destOffset,
    GtU64 size
) {
    vkQueueWaitIdle(queue);

    GtVkCmdBuffer tempCommandBuffer;
    gontiVkCommandBufferAllocateAndBeginSingleUse(context, pool, &tempCommandBuffer);

    VkBufferCopy copyRegion;
    copyRegion.srcOffset = sourceOffset;
    copyRegion.dstOffset = destOffset;
    copyRegion.size = size;

    vkCmdCopyBuffer(tempCommandBuffer.handle, source, dest, 1, &copyRegion);
    gontiVkCommandBufferEndSingleUse(context, pool, &tempCommandBuffer, queue);
}

/* 
* VOID* 
*/
void* gontiVkBufferLockMemory(GtVkContext* context, GtVkBuffer* buffer, GtU64 offset, GtU64 size, GtU32 flags) {
    void* ret;
    GTVK_CHECK(vkMapMemory(context->device.logicalDevice, buffer->memory, offset, size, flags, &ret));
    return ret;
}