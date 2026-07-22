//
// Created by nutellis on 18/07/2026.
//

#ifndef ALKYONERENDERENGINE_VULKANBUFFERTRANSFORMS_H
#define ALKYONERENDERENGINE_VULKANBUFFERTRANSFORMS_H
#include "core/PODTypes.h"
#include "rhi/descriptors/BufferDesc.h"
#include "utilities/TypeUtilities.h"
#include "vk_mem_alloc.h"

namespace Transformations {
    inline VkBufferUsageFlags VulkanBufferUsageFlags(BufferTypeBits flagsB) {

        const uint32 flag = Underlying(flagsB);
        VkBufferUsageFlags flags = 0;

        if (flag & Underlying(BufferTypeBits::VERTEX_BUFFER)) flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        if (flag & Underlying(BufferTypeBits::INDEX_BUFFER)) flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        if (flag & Underlying(BufferTypeBits::UNIFORM_BUFFER)) flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        if (flag & Underlying(BufferTypeBits::STORAGE_BUFFER)) flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
        if (flag & Underlying(BufferTypeBits::SHADER_DATA_BUFFER)) flags |= VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;
        if (flag & Underlying(BufferTypeBits::TRANSFER_SRC)) flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        if (flag & Underlying(BufferTypeBits::TRANSFER_DST)) flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        return flags;
    };

    inline VmaAllocationCreateFlags VulkanMemoryUsageFlags(MemoryUsageStrategy strategy) {
        switch (strategy) {
            case MemoryUsageStrategy::GPU_ONLY:
                return 0;
            case MemoryUsageStrategy::CPU_TO_GPU:
                return VMA_ALLOCATION_CREATE_MAPPED_BIT |
                    VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
                    VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT;
            default:
                return VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
        }
    }
}

#endif //ALKYONERENDERENGINE_VULKANBUFFERTRANSFORMS_H