//
// Created by nutellis on 29/07/2026.
//

#ifndef ALKYONERENDERENGINE_VULKANRESOURCES_H
#define ALKYONERENDERENGINE_VULKANRESOURCES_H

#include <volk.h>
#include <vk_mem_alloc.h>

#include "rhi/descriptors/BufferDesc.h"
#include "rhi/vulkan/VulkanBuffer.h"
#include "rhi/vulkan/VulkanBufferTransforms.h"
#include "spdlog/spdlog.h"

namespace VulkanResources {
    inline VulkanBuffer CreateBuffer(const BufferDesc &desc, VmaAllocator allocator) {

        VkBuffer buffer = VK_NULL_HANDLE;
        VmaAllocation allocation = VK_NULL_HANDLE;

        VkBufferCreateInfo bufferCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size = desc.size,
            .usage = Transformations::VulkanBufferUsageFlags(desc.usageFlags),
            .sharingMode = desc.sharingMode == SharingMode::EXCLUSIVE ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT
        };

        VmaAllocationCreateInfo vmaAllocationCreateInfo = {
            .flags = Transformations::VulkanMemoryUsageFlags(desc.memoryUsageStrategy),
            .usage = VMA_MEMORY_USAGE_AUTO
        };
        VmaAllocationInfo allocInfo;
        VkResult result = vmaCreateBuffer(allocator, &bufferCreateInfo, &vmaAllocationCreateInfo, &buffer, &allocation, &allocInfo);
        if (result != VK_SUCCESS)
        {
            // buffer failed to init
            spdlog::error("Failed to create Vulkan buffer {}", (int)result);
            return VulkanBuffer {} ;
        }

        return VulkanBuffer {
            allocInfo.pMappedData,
            buffer,
            allocation
        };
    }

    inline void DestroyBuffer(VulkanBuffer & vulkanBuffer, VmaAllocator allocator)
    {
        if (vulkanBuffer.buffer != VK_NULL_HANDLE)
        {
            vmaDestroyBuffer(allocator, vulkanBuffer.buffer, vulkanBuffer.allocation);
            vulkanBuffer.buffer = VK_NULL_HANDLE;
            vulkanBuffer.allocation = VK_NULL_HANDLE;
            vulkanBuffer.mappedData = nullptr;
        }
    }

}
#endif //ALKYONERENDERENGINE_VULKANRESOURCES_H