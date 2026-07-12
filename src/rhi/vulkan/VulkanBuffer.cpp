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
        if (flag & Underlying(BufferTypeBits::TRANSFER_SRC)) flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        if (flag & Underlying(BufferTypeBits::TRANSFER_DST)) flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        return flags;
    };

    VmaAllocationCreateFlags VulkanMemoryUsageFlags(MemoryUsageStrategy strategy) {
        switch (strategy) {
            case MemoryUsageStrategy::GPU_ONLY:
                return 0;
            case MemoryUsageStrategy::CPU_TO_GPU:
                return VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
                       VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT;
            default:
                return VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
        }
    }
}


VulkanBuffer::VulkanBuffer(VulkanDevice* device) : device(device)
{
}

VulkanBuffer::~VulkanBuffer()
{
    VulkanBuffer::Terminate();
}

VulkanBuffer::VulkanBuffer(VulkanBuffer&& other) noexcept :
device(other.device),
buffer(other.buffer),
allocation(other.allocation),
mappedData(other.mappedData)
{
    other.buffer = VK_NULL_HANDLE;
    other.allocation = VK_NULL_HANDLE;
    other.mappedData = nullptr;
}

VulkanBuffer& VulkanBuffer::operator=(VulkanBuffer&& other) noexcept
{
    if (this != &other)
    {
        Terminate();

        device = other.device;
        buffer = other.buffer;
        allocation = other.allocation;
        mappedData = other.mappedData;

        other.buffer = VK_NULL_HANDLE;
        other.allocation = VK_NULL_HANDLE;
        other.mappedData = nullptr;
    }
    return *this;
}


bool VulkanBuffer::Initialize(BufferDesc desc)
{
    VkBufferCreateInfo bufferCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = desc.size,
        .usage = VulkanBufferUsageFlags(desc.usageFlags),
        .sharingMode = desc.sharingMode == SharingMode::EXCLUSIVE ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT
    };

    VmaAllocationCreateInfo vmaAllocationCreateInfo = {
        .flags =  VulkanMemoryUsageFlags(desc.memoryUsageStrategy),
        .usage = VMA_MEMORY_USAGE_AUTO
    };

    VkResult result = vmaCreateBuffer(device->GetAllocator(), &bufferCreateInfo, &vmaAllocationCreateInfo, &buffer, &allocation, nullptr);
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
    if (buffer != VK_NULL_HANDLE)
    {
        vmaDestroyBuffer(device->GetAllocator(), buffer, allocation);
        buffer = VK_NULL_HANDLE;
        allocation = VK_NULL_HANDLE;
        mappedData = nullptr;
    }
}

void VulkanBuffer::Map()
{
    if (mappedData != nullptr)
    {
        spdlog::warn("VulkanBuffer: Buffer is already mapped");
        return;
    }

    vmaMapMemory(device->GetAllocator(), allocation, &mappedData);
}

void VulkanBuffer::Unmap()
{
    if (mappedData == nullptr)
    {
        spdlog::warn("VulkanBuffer: Buffer is not mapped");
        return;
    }

    vmaUnmapMemory(device->GetAllocator(), allocation);
    mappedData = nullptr;
}

VkBuffer VulkanBuffer::GetVkBuffer() const
{
    return buffer;
}
