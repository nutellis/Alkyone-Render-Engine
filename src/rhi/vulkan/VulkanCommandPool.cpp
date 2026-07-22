//
// Created by Nutellis on 11-Feb-26.
//

#include <rhi/vulkan/VulkanCommandPool.h>

#include "volk.h"

#include "rhi/vulkan/VulkanCommandBuffer.h"
#include "rhi/vulkan/VulkanDefinitions.h"
#include "rhi/vulkan/VulkanQueue.h"
#include "spdlog/spdlog.h"

VulkanCommandPool::VulkanCommandPool(VulkanDevice& device, uint32 queueFamilyIndex):
    device(device),
    queueFamilyIndex(queueFamilyIndex)
{
}

VulkanCommandPool::~VulkanCommandPool()
{
}

bool VulkanCommandPool::Initialize()
{
    VkCommandPoolCreateInfo poolCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = queueFamilyIndex
    };

    if (vkCreateCommandPool(device.GetLogicalDevice(), &poolCreateInfo,nullptr, &commandPool) != VK_SUCCESS)
    {
        spdlog::error("Failed to create Command Pool");
        return false;
    }

    spdlog::info("Command Pool Created");
    return true;
}

void VulkanCommandPool::Terminate()
{
    vkDeviceWaitIdle(device.GetLogicalDevice());

    vkDestroyCommandPool(device.GetLogicalDevice(), commandPool, nullptr);
}

ICommandBuffer* VulkanCommandPool::AllocateCommandBuffer(CommandBufferLevel bufferLevel)
{
    //this can become one call and allocate multiple buffers.
    VkCommandBufferAllocateInfo bufferAllocateInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = commandPool,
        .level = VulkanCommandBufferLevel[bufferLevel],
        .commandBufferCount = 1
    };

    VkCommandBuffer buffer;
    if (vkAllocateCommandBuffers(device.GetLogicalDevice(), &bufferAllocateInfo, &buffer) != VK_SUCCESS)
    {
        spdlog::error("Failed to allocate Command Buffer");
        return nullptr;
    }

    commandBuffer = new VulkanCommandBuffer(device, buffer, bufferLevel);
    spdlog::info("Command Buffer Created");
    commandBuffer->Initialize(); // does nothing now. For good measure call it, its fine.
    //commandBuffers.push_back(vulkanBuffer);

    return commandBuffer;
}

VulkanCommandBuffer& VulkanCommandPool::GetCommandBuffer() const
{
    return *commandBuffer;
}

VkCommandPool VulkanCommandPool::GetVkCommandPool() const
{
    return commandPool;
}

