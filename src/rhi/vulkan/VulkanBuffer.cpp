//
// Created by Nutellis on 08-Mar-26.
//

#include "rhi/vulkan/VulkanBuffer.h"
#include <utilities/TypeUtilities.h>

#include "rhi/vulkan/VulkanDevice.h"
#include "spdlog/spdlog.h"

namespace
{
    VkBufferUsageFlags VulkanBufferUsageFlags(BufferTypeBits flagsB) {

        uint32 flag = Underlying(flagsB);
        VkBufferUsageFlags flags = 0;

        if (flag & Underlying(BufferTypeBits::VERTEX_BUFFER)) flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        if (flag & Underlying(BufferTypeBits::INDEX_BUFFER)) flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        if (flag & Underlying(BufferTypeBits::UNIFORM_BUFFER)) flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        if (flag & Underlying(BufferTypeBits::STORAGE_BUFFER)) flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
        if (flag & Underlying(BufferTypeBits::SHADER_DATA_BUFFER)) flags |= VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;

        return flags;
    };
}


VulkanBuffer::VulkanBuffer(VulkanDevice& device) : device(device)
{
}

VulkanBuffer::~VulkanBuffer()
{
}

bool VulkanBuffer::Initialize(BufferDesc desc)
{
    VkBufferCreateInfo bufferCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = desc.size,
        .usage = VulkanBufferUsageFlags(desc.usageFlags),
        .sharingMode = desc.sharingMode == SharingMode::EXCLUSIVE ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT
    };

    //TODO: add a flag on the descriptor for this
    VmaAllocationCreateInfo vmaAllocationCreateInfo = {
        .flags =  VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
        VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT |
        VMA_ALLOCATION_CREATE_MAPPED_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO
    };

    VkResult result = vmaCreateBuffer(device.GetAllocator(), &bufferCreateInfo, &vmaAllocationCreateInfo, &buffer, &allocation, nullptr);
    if (result != VK_SUCCESS)
    {
        // buffer failed to init
        spdlog::error("Failed to create Vulkan buffer {}", (int)result);
        return false;
    }

    return true;
}

void VulkanBuffer::Terminate()
{
    vmaDestroyBuffer(device.GetAllocator(), buffer, allocation);
}

void VulkanBuffer::CopyData(const void* data, uint64 size, uint64 offset)
{
    void* mappedData;
    vmaMapMemory(device.GetAllocator(), allocation, &mappedData);
    memcpy(mappedData, data, size);

    vmaUnmapMemory(device.GetAllocator(), allocation);
}
