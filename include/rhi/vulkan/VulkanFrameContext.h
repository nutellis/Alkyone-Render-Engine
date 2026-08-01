//
// Created by Nutellis on 19-Feb-26.
//

#ifndef ALKYONERENDERENGINE_VULKANFRAME_H
#define ALKYONERENDERENGINE_VULKANFRAME_H

#include "rhi/core/IFrameContext.h"
#include <volk.h>

#include <core/PODTypes.h>

class VulkanSwapchain;
class VulkanQueue;
class VulkanDevice;

class VulkanCommandPool;

struct VulkanFrameContext: public IFrameContext
{

    VkSemaphore imageAcquiredSemaphore; //semaphore for getting swapchain image index
    VkSemaphore imageRenderedSemaphore; //
    VkFence inFlightFence;

    VkCommandPool commandPool = VK_NULL_HANDLE;
    VkCommandBuffer commandBuffer = VK_NULL_HANDLE;

   // DeletionQueue deletionQueue;
};

#endif //ALKYONERENDERENGINE_VULKANFRAME_H
