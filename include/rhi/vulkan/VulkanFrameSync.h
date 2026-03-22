//
// Created by Nutellis on 19-Feb-26.
//

#ifndef ALKYONERENDERENGINE_VULKANFRAME_H
#define ALKYONERENDERENGINE_VULKANFRAME_H

#include "rhi/core/IFrameSync.h"
#include <volk.h>

#include <core/PODTypes.h>

class VulkanSwapchain;
class VulkanQueue;
class VulkanDevice;

class VulkanCommandPool;

class VulkanFrameSync: public IFrameSync
{
public:
    VulkanFrameSync() = delete;

    VulkanFrameSync(VulkanDevice& device, VulkanSwapchain& swapchain);
    ~VulkanFrameSync() override;
    bool Initialize() override;
    void Terminate() override;
    uint32 BeginSynchronize() override;
    void ResetSyncObjects() override;
    void EndSynchronize() override;

    VkSemaphore imageAcquiredSemaphore[FRAMES_IN_FLIGHT]; //semaphore for getting swapchain image index
    VkSemaphore imageRenderedSemaphore[FRAMES_IN_FLIGHT]; //
    VkFence inFlightFences[FRAMES_IN_FLIGHT];

    uint32 currentFrame = 0;
    uint32 imageIndex = 0; //swapchain image index

private:
    // device holds the queues
    VulkanDevice & device;
    VulkanSwapchain & swapchain;
};

#endif //ALKYONERENDERENGINE_VULKANFRAME_H
