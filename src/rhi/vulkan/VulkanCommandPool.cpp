//
// Created by Nutellis on 11-Feb-26.
//

#include <rhi/vulkan/VulkanCommandPool.h>

#include "volk.h"

#include "rhi/vulkan/VulkanCommandBuffer.h"
#include "rhi/vulkan/VulkanDefinitions.h"
#include "rhi/vulkan/VulkanQueue.h"
#include "spdlog/spdlog.h"


// ICommandBuffer* VulkanCommandPool::AllocateCommandBuffer(CommandBufferLevel bufferLevel)
// {
//     //this can become one call and allocate multiple buffers.
//     VkCommandBufferAllocateInfo bufferAllocateInfo = {
//         .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
//         .pNext = nullptr,
//         .commandPool = commandPool,
//         .level = VulkanCommandBufferLevel[bufferLevel],
//         .commandBufferCount = 1
//     };
//
//     VkCommandBuffer buffer;
//     if (vkAllocateCommandBuffers(device.GetLogicalDevice(), &bufferAllocateInfo, &buffer) != VK_SUCCESS)
//     {
//         spdlog::error("Failed to allocate Command Buffer");
//         return nullptr;
//     }
//
//     commandBuffer = new VulkanCommandBuffer(device, buffer, bufferLevel);
//     spdlog::info("Command Buffer Created");
//     commandBuffer->Initialize(); // does nothing now. For good measure call it, its fine.
//     //commandBuffers.push_back(vulkanBuffer);
//
//     return commandBuffer;
// }


