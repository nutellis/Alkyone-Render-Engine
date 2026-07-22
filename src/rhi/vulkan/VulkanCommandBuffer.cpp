//
// Created by Nutellis on 18-Feb-26.
//

#include <rhi/vulkan/VulkanCommandBuffer.h>

#include "rhi/vulkan/VulkanDefinitions.h"
#include "rhi/vulkan/VulkanImage.h"

VulkanCommandBuffer::VulkanCommandBuffer(VulkanDevice& device, VkCommandBuffer buffer, CommandBufferLevel bufferLevel) :
device(device),
commandBuffer(buffer),
bufferLevel(VulkanCommandBufferLevel[bufferLevel])
{
}

VulkanCommandBuffer::~VulkanCommandBuffer()
{
}

bool VulkanCommandBuffer::Initialize()
{
    return true;
}

void VulkanCommandBuffer::Terminate()
{
}

VkCommandBuffer VulkanCommandBuffer::GetVkCommandBuffer() const
{
    return commandBuffer;
}

void VulkanCommandBuffer::Begin()
{
    VkCommandBufferBeginInfo cbOneTimeBI{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    };

    vkBeginCommandBuffer(commandBuffer, &cbOneTimeBI);
}

void VulkanCommandBuffer::Reset()
{
    vkResetCommandBuffer(commandBuffer, 0);
}

void VulkanCommandBuffer::End()
{
    //TODO: error handling
   vkEndCommandBuffer(commandBuffer);
}

void VulkanCommandBuffer::Barrier(const ImageBarrier barrier)
{
    VkImageMemoryBarrier2 imageBarrier {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
        .pNext = nullptr,
        .srcStageMask = VulkanStageFlags(barrier.sourceStageMask),
        .srcAccessMask = VulkanAccessFlags(barrier.sourceAccessMask),
        .dstStageMask = VulkanStageFlags(barrier.destStageMask),
        .dstAccessMask = VulkanAccessFlags(barrier.destAccessMask),
        .oldLayout = VulkanTransitionFlags[barrier.sourceState],
        .newLayout = VulkanTransitionFlags[barrier.destState],
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = static_cast<VulkanImage*>(barrier.image)->GetImage(),
        .subresourceRange = {
            .aspectMask = VulkanImageAspectFlags(barrier.aspectMask),
            .baseMipLevel = barrier.baseMipLevel,
            .levelCount = barrier.levelCount,
            .baseArrayLayer = barrier.baseArrayLayer,
            .layerCount = barrier.layerCount
        }
    };

    VkDependencyInfo dependencyInfo {
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .pNext = nullptr,
        .imageMemoryBarrierCount = 1,
        .pImageMemoryBarriers = &imageBarrier
    };

    vkCmdPipelineBarrier2(commandBuffer, &dependencyInfo);
}
