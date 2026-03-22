//
// Created by Nutellis on 19-Feb-26.
//

#include <rhi/vulkan/VulkanFrameSync.h>
#include <rhi/vulkan/VulkanCommandPool.h>

#include "spdlog/spdlog.h"

VulkanFrameSync::VulkanFrameSync(VulkanDevice& device, VulkanSwapchain& swapchain) : device(device), swapchain(swapchain)
{
}

VulkanFrameSync::~VulkanFrameSync()
{
}

bool VulkanFrameSync::Initialize()
{
    VkFenceCreateInfo fenceCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };

    VkSemaphoreCreateInfo semaphoreCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = nullptr
    };

    //TODO: error handling
    for (int i = 0; i < FRAMES_IN_FLIGHT; ++i)
    {
        vkCreateFence(device.GetLogicalDevice(), &fenceCreateInfo, nullptr, &inFlightFences[i]);

        vkCreateSemaphore(device.GetLogicalDevice(), &semaphoreCreateInfo, nullptr, &imageAcquiredSemaphore[i]);
        vkCreateSemaphore(device.GetLogicalDevice(), &semaphoreCreateInfo, nullptr, &imageRenderedSemaphore[i]);

    }
    return true;
}

void VulkanFrameSync::Terminate()
{
    for (int i = 0; i < FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(device.GetLogicalDevice(), imageRenderedSemaphore[i], nullptr);
        vkDestroySemaphore(device.GetLogicalDevice(), imageAcquiredSemaphore[i], nullptr);
        vkDestroyFence(device.GetLogicalDevice(), inFlightFences[i], nullptr);
    }
}

uint32 VulkanFrameSync::BeginSynchronize()
{
     vkWaitForFences(device.GetLogicalDevice(), 1, &inFlightFences[currentFrame], true, UINT64_MAX);

    //TODO: error handling
     VkResult result = vkAcquireNextImageKHR(
         device.GetLogicalDevice(),
         swapchain.GetVkSwapchain(),
         UINT64_MAX,
         imageAcquiredSemaphore[currentFrame],
         nullptr, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || swapchain.IsFrameBufferResized()) {

        swapchain.ResetIsFrameBufferResized();
        swapchain.RecreateSwapChain();
        ResetSyncObjects();
        return 666;
    }
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        spdlog::error("Failed to acquire swapchain image!");
        ResetSyncObjects();
        return 666;
    }

     vkResetFences(device.GetLogicalDevice(), 1, &inFlightFences[currentFrame]);

    return currentFrame;
}

void VulkanFrameSync::ResetSyncObjects() {
    Terminate();
    Initialize();
}

void VulkanFrameSync::EndSynchronize()
{
    currentFrame = (currentFrame + 1) % FRAMES_IN_FLIGHT;
}
