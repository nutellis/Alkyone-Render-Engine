//
// Created by nutellis on 29/07/2026.
//

#include "VulkanFrameManager.h"

#include "rhi/vulkan/VulkanDevice.h"
#include "rhi/vulkan/VulkanQueue.h"
#include "rhi/vulkan/VulkanSwapchain.h"
#include "rhi/vulkan/operations/VulkanCommands.h"

VulkanFrameManager::VulkanFrameManager(VulkanDevice &device, VulkanSwapchain &swapchain)
    : RHIFrameManager(), device(device), swapchain(swapchain) {
}

VulkanFrameManager::~VulkanFrameManager() = default;

bool VulkanFrameManager::Initialize() {
    VkDevice logicalDevice = device.GetLogicalDevice();

    // initialize semaphore and fence
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
        vkCreateFence(logicalDevice, &fenceCreateInfo, nullptr, &frames[i].inFlightFence);

        vkCreateSemaphore(logicalDevice, &semaphoreCreateInfo, nullptr, &frames[i].imageAcquiredSemaphore);
        vkCreateSemaphore(logicalDevice, &semaphoreCreateInfo, nullptr, &frames[i].imageRenderedSemaphore);

        frames[i].commandPool = VulkanCommands::CreateCommandPool(logicalDevice, device.GetGraphicsQueue()->GetFamilyIndex());
        frames[i].commandBuffer = VulkanCommands::CreateCommandBuffer(logicalDevice, frames[i].commandPool, COMMAND_BUFFER_LEVEL_PRIMARY);
    }
    return true;
}

void VulkanFrameManager::Terminate() {
    VkDevice logicalDevice = device.GetLogicalDevice();

    for (int i = 0; i < FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(logicalDevice  , frames[i].imageRenderedSemaphore, nullptr);
        vkDestroySemaphore(logicalDevice, frames[i].imageAcquiredSemaphore, nullptr);
        vkDestroyFence(logicalDevice, frames[i].inFlightFence, nullptr);

        VulkanCommands::DestroyCommandPool(logicalDevice, frames[i].commandPool);
    }
}

bool VulkanFrameManager::BeginSynchronize() {

    VulkanFrameContext & frame = frames[currentFrame];

    vkWaitForFences(device.GetLogicalDevice(), 1, &frame.inFlightFence, true, UINT64_MAX);

    vkResetCommandPool(device.GetLogicalDevice(), frame.commandPool, 0);

    // //TODO: error handling
    VkResult result = vkAcquireNextImageKHR(
        device.GetLogicalDevice(),
        swapchain.GetVkSwapchain(),
        UINT64_MAX,
        frame.imageAcquiredSemaphore,
        nullptr,
        &imageIndex);
    //
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || swapchain.IsFrameBufferResized()) {

        swapchain.ResetIsFrameBufferResized();

        //we should halt everything till the resources are free to recreate
        vkDeviceWaitIdle(device.GetLogicalDevice());
        swapchain.RecreateSwapChain();
        return false;
    }
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        spdlog::error("Failed to acquire swapchain image!");
        //ResetSyncObjects();
        return false;
    }

    vkResetFences(device.GetLogicalDevice(), 1, &frame.inFlightFence);

    //set the command buffer to write state. also TODO: move this to a separate function
    VkCommandBufferBeginInfo cbOneTimeBI{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    };

    vkBeginCommandBuffer(frames[currentFrame].commandBuffer, &cbOneTimeBI);

    return true;
}

void VulkanFrameManager::SubmitQueue() {

    //TODO: move this to its own function
    vkEndCommandBuffer( frames[currentFrame].commandBuffer);

    //create the submit info for the queue and handle synchronization

    //this might need to move into a function
    VkSemaphoreSubmitInfo waitSemaphoreInfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .pNext = nullptr,
        .semaphore = frames[currentFrame].imageAcquiredSemaphore,
        .value = 0,
        .stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
        .deviceIndex = 0
    };

    //same for this
    VkSemaphoreSubmitInfo  singalSemaphoreInfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .pNext = nullptr,
        .semaphore = frames[currentFrame].imageRenderedSemaphore,
        .value = 0,
        .stageMask = VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT, // check this
        .deviceIndex = 0,

    };

    VkCommandBufferSubmitInfo submitCommandBufferInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
        .pNext = nullptr,
        .commandBuffer = frames[currentFrame].commandBuffer,
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

    vkQueueSubmit2(device.GetGraphicsQueue()->GetVkQueue(), 1, &submitInfo, frames[currentFrame].inFlightFence);
}

void VulkanFrameManager::Present() {
    //wait for rendering semaphores

    VkSwapchainKHR swapchains[] = { swapchain.GetVkSwapchain() };

    //present info
    VkPresentInfoKHR presentInfo = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &frames[currentFrame].imageRenderedSemaphore,
        .swapchainCount = 1,
        .pSwapchains =  swapchains,
        .pImageIndices = &imageIndex
    };
    VkQueue queue = device.GetGraphicsQueue()->GetVkQueue();
    VkResult result = vkQueuePresentKHR(queue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || swapchain.IsFrameBufferResized()) {

        swapchain.ResetIsFrameBufferResized();

        //we should halt everything till the resources are free to recreate
        vkDeviceWaitIdle(device.GetLogicalDevice());
        swapchain.RecreateSwapChain();
    }
}


void VulkanFrameManager::EndSynchronize() {
    currentFrame = (currentFrame + 1) % FRAMES_IN_FLIGHT;
}

VkCommandBuffer VulkanFrameManager::GetCurrentCommandBuffer() const {
    return frames[currentFrame].commandBuffer;
}
