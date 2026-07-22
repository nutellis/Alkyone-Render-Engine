//
// Created by Nutellis on 13-Feb-26.
//

#include <rhi/vulkan/VulkanQueue.h>

#include "rhi/vulkan/VulkanCommandBuffer.h"
#include "rhi/vulkan/VulkanCommandPool.h"
#include "rhi/vulkan/VulkanDefinitions.h"
#include "rhi/vulkan/VulkanDevice.h"
#include "rhi/vulkan/VulkanFrameSync.h"
#include "spdlog/spdlog.h"

VulkanQueue::VulkanQueue(VulkanDevice & device, uint32 inFamilyIndex, CommandQueueType inType) :
    parentDevice(device),
    familyIndex(inFamilyIndex),
    queueIndex(0),
    queueType(VulkanQueueFlags[inType])
{
}

VulkanQueue::~VulkanQueue()
{
}

bool VulkanQueue::Initialize(size_t poolSize = FRAMES_IN_FLIGHT)
{
    VkDevice logicalDevice = parentDevice.GetLogicalDevice();
    if (logicalDevice == VK_NULL_HANDLE)
    {
        spdlog::error("Alkyone RHI: Critical failure during Vulkan Queue initialization");
        return false;
    }

    vkGetDeviceQueue(logicalDevice, familyIndex, queueIndex, &queue);

    if (queue == VK_NULL_HANDLE)
    {
        spdlog::error("Alkyone RHI: Critical failure during Vulkan Queue initialization");
        return false;
    }

    pools.resize(poolSize);
    for (int i=0; i < poolSize; ++i)
    {
        VulkanCommandPool * pool = new VulkanCommandPool(parentDevice, familyIndex);

        //TODO: error handling
        pool->Initialize();

        pools[i]= pool;
    }


    return true;
}

void VulkanQueue::Terminate()
{
    for (int i=0; i< pools.size(); ++i)
    {
        pools[i]->Terminate();
        delete pools[i];
    }
}

VkQueue VulkanQueue::GetVkQueue()
{
    return queue;
}

VkQueueFlags VulkanQueue::GetVkQueueFlags() const
{
    return queueType;
}

uint32 VulkanQueue::GetFamilyIndex() const
{
    return familyIndex;
}

VulkanCommandPool* VulkanQueue::GetCommandPool(uint32 index)
{
    if (index >= pools.size())
    {
        spdlog::error("Alkyone RHI: Invalid frame index when retrieving command pool");
        return nullptr;
    }

    return pools[index];
}

void VulkanQueue::AllocateCommandBuffers(const uint32 count)
{
    for (uint32 i = 0; i < count; ++i)
    {
        pools[i]->AllocateCommandBuffer(COMMAND_BUFFER_LEVEL_PRIMARY);
    }
}

//TODO: i probably need a SubmitImmediate(ICommandBuffer & cmdBuffer)
void VulkanQueue::SubmitCommandBuffer(IFrameSync & sync, ICommandBuffer & cmdBuffer)
{
    const VulkanCommandBuffer & vkCmdBuffer = static_cast<VulkanCommandBuffer&>(cmdBuffer);
    const VkCommandBuffer vkCmd = vkCmdBuffer.GetVkCommandBuffer();

    const VulkanFrameSync & vkFrameSync = static_cast<VulkanFrameSync&>(sync);

    //create the submit info for the queue and handle synchronization

    //this might need to move into a function
    VkSemaphoreSubmitInfo waitSemaphoreInfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .pNext = nullptr,
        .semaphore = vkFrameSync.imageAcquiredSemaphore[vkFrameSync.currentFrame],
        .value = 1,
        .stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
        .deviceIndex = 0
    };

    //same for this
    VkSemaphoreSubmitInfo  singalSemaphoreInfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .pNext = nullptr,
        .semaphore = vkFrameSync.imageRenderedSemaphore[vkFrameSync.currentFrame],
        .value = 1,
        .stageMask = VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT, // check this
        .deviceIndex = 0,

    };

     VkCommandBufferSubmitInfo submitCommandBufferInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
        .pNext = nullptr,
        .commandBuffer = vkCmd,
        .deviceMask = 0
    };
     VkSubmitInfo2 submitInfo{
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .pNext = nullptr,

        .waitSemaphoreInfoCount = 1,
        .pWaitSemaphoreInfos = &waitSemaphoreInfo,
        .commandBufferInfoCount = 1,
        .pCommandBufferInfos = &submitCommandBufferInfo,

        .signalSemaphoreInfoCount = 1,
        .pSignalSemaphoreInfos = &singalSemaphoreInfo,
     };

    vkQueueSubmit2(queue, 1, &submitInfo, vkFrameSync.inFlightFences[vkFrameSync.currentFrame]);
}

