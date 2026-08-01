//
// Created by nutellis on 30/07/2026.
//

#ifndef ALKYONERENDERENGINE_VULKANCOMMANDS_H
#define ALKYONERENDERENGINE_VULKANCOMMANDS_H

#include <volk.h>

#include "rhi/vulkan/VulkanCommandBuffer.h"
#include "rhi/vulkan/VulkanCommandPool.h"
#include "rhi/vulkan/VulkanDefinitions.h"
#include "spdlog/spdlog.h"

namespace VulkanCommands {
    inline VkCommandPool CreateCommandPool(VkDevice device, uint32 queueFamilyIndex) {

        VkCommandPool commandPool = VK_NULL_HANDLE;

        VkCommandPoolCreateInfo poolCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = queueFamilyIndex
        };

        if (vkCreateCommandPool(device, &poolCreateInfo,nullptr, &commandPool) != VK_SUCCESS)
        {
            spdlog::error("Failed to create Command Pool");
            return VK_NULL_HANDLE;
        }

        spdlog::info("Command Pool Created");
        return commandPool;
    }

    inline void DestroyCommandPool(VkDevice device, VkCommandPool commandPool)
    {
        vkDeviceWaitIdle(device);

        vkDestroyCommandPool(device, commandPool, nullptr);
    }

    inline VkCommandBuffer CreateCommandBuffer(VkDevice device, VkCommandPool commandPool, CommandBufferLevel bufferLevel)
    {
        VkCommandBuffer buffer = VK_NULL_HANDLE;

        //this can become one call and allocate multiple buffers.
        VkCommandBufferAllocateInfo bufferAllocateInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = nullptr,
            .commandPool = commandPool,
            .level = VulkanCommandBufferLevel[bufferLevel],
            .commandBufferCount = 1
        };

        if (vkAllocateCommandBuffers(device, &bufferAllocateInfo, &buffer) != VK_SUCCESS)
        {
            spdlog::error("Failed to allocate Command Buffer");
            return VK_NULL_HANDLE;
        }

        return buffer;
    }

    inline void BeginCommandBuffer(VkCommandBuffer commandBuffer) {
        VkCommandBufferBeginInfo cbOneTimeBI = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
        };

        vkBeginCommandBuffer(commandBuffer, &cbOneTimeBI);
    }

    // i dont intent to use those. DO NOT USE
    inline void EndCommandBuffer(VkCommandBuffer commandBuffer) {
        vkEndCommandBuffer(commandBuffer);
    }

    inline void ResetCommandBuffer(VkCommandBuffer commandBuffer) {
        vkResetCommandBuffer(commandBuffer, 0);
    }

}
#endif //ALKYONERENDERENGINE_VULKANCOMMANDS_H