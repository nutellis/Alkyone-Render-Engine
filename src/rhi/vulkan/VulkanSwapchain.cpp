//
// Created by Nutellis on 13-Feb-26.
//

#include <rhi/vulkan/VulkanSwapchain.h>

#include "rhi/vulkan/VulkanDevice.h"
#include "rhi/vulkan/VulkanFrameSync.h"
#include "rhi/vulkan/VulkanQueue.h"
#include "spdlog/spdlog.h"

VulkanSwapchain::VulkanSwapchain(VulkanDevice & inParent, ARWindow & inWindow)
: parent(inParent), window(inWindow)
{
}

VulkanSwapchain::~VulkanSwapchain()
{
}

bool VulkanSwapchain::Initialize()
{
    instance = parent.GetInstance();

    window.CreateVulkanSurface(instance, &surface);

    if (CheckSwapChainSupport()
        && CreateSwapChain()
        && CreateImageViews() == false)
    {
        spdlog::error("Alkyone RHI: Critical failure during Vulkan SwapChain initialization");
        return false;
    }

    return true;
}

void VulkanSwapchain::Terminate()
{
    CleanupSwapChain();

    vkDestroySurfaceKHR(instance, surface, nullptr);
}

void VulkanSwapchain::RecreateSwapChain()
{
    //wait
    parent.WaitIdle();

    CleanupSwapChain();

    if (CreateSwapChain()
        && CreateImageViews() == false)
    {
        spdlog::error("Alkyone RHI: Critical failure during Vulkan SwapChain recreation");
    }

}

void VulkanSwapchain::CleanupSwapChain()
{
    for (auto imageView : swapChainImageViews) {
        vkDestroyImageView(parent.GetLogicalDevice(), imageView, nullptr);
    }
    swapChainImageViews.clear();

    vkDestroySwapchainKHR(parent.GetLogicalDevice(), swapChain, nullptr);
}

void VulkanSwapchain::Present(IFrameSync & frameSync)
{
    //wait for rendering semaphores
    const VulkanFrameSync & frame = static_cast<VulkanFrameSync&>(frameSync);


    //present info
    VkPresentInfoKHR presentInfo = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &frame.imageRenderedSemaphore[frame.currentFrame],
        .swapchainCount = 1,
        .pSwapchains = &swapChain,
        .pImageIndices = &frame.imageIndex
    };
    VkQueue queue = parent.GetGraphicsQueue()->GetVkQueue();
    vkQueuePresentKHR(queue, &presentInfo);
}

bool VulkanSwapchain::IsFrameBufferResized() const
{
    return window.framebufferResized;
}

void VulkanSwapchain::ResetIsFrameBufferResized() const
{
    window.framebufferResized = false;
}

bool VulkanSwapchain::CheckSwapChainSupport()
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(parent.GetPhysicalDevice(), surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(parent.GetPhysicalDevice(), surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(parent.GetPhysicalDevice(), surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(parent.GetPhysicalDevice(), surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(parent.GetPhysicalDevice(), surface, &presentModeCount, details.presentModes.data());
    }

    if (details.formats.empty() && details.presentModes.empty())
    {
        swapChainSupportDetails = {};

        spdlog::error("Device does not support present swap chain");
        return false;
    }

    swapChainSupportDetails = details;
    return true;
}

VkSurfaceKHR VulkanSwapchain::GetVkSurface() const
{
    return surface;
}

VkSwapchainKHR VulkanSwapchain::GetVkSwapchain() const
{
    return swapChain;
}

VkImage VulkanSwapchain::GetVkImage(uint32_t index) const
{
    if (index >= swapChainImages.size())
    {
        spdlog::error("Swapchain image index out of bounds");
        return VK_NULL_HANDLE;
    }

    return swapChainImages[index];
}

VkImageView VulkanSwapchain::GetVkImageView(uint32_t index) const
{
    if (index >= swapChainImageViews.size())
    {
        spdlog::error("Swapchain image view index out of bounds");
        return VK_NULL_HANDLE;
    }

    return swapChainImageViews[index];
}

VkExtent2D VulkanSwapchain::GetVkExtent() const
{
    return swapChainExtent;
}


bool VulkanSwapchain::CreateSwapChain()
{
    //const VkFormat imageFormat{ VK_FORMAT_B8G8R8A8_SRGB };
    imageFormat = VK_FORMAT_B8G8R8A8_SRGB;

    swapChainExtent = swapChainSupportDetails.capabilities.currentExtent;

    VkSwapchainCreateInfoKHR swapChainCreateInfo {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = surface,
        .minImageCount = swapChainSupportDetails.capabilities.minImageCount,
        .imageFormat = imageFormat,
        .imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR,
        .imageExtent{
            .width = swapChainExtent.width,
            .height = swapChainExtent.height
        },
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, // VK_IMAGE_USAGE_TRANSFER_DST_BIT is for clear
        .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = VK_PRESENT_MODE_FIFO_KHR
    };

    if (vkCreateSwapchainKHR(parent.GetLogicalDevice(), &swapChainCreateInfo, nullptr, &swapChain) != VK_SUCCESS)
    {
        spdlog::error("Failed to create swap chain");
        return false;
    }


    uint32 imageCount = 0;

    vkGetSwapchainImagesKHR(parent.GetLogicalDevice(), swapChain, &imageCount, nullptr);

    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(parent.GetLogicalDevice(), swapChain, &imageCount, swapChainImages.data());

    //TODO: check if its created
    std::vector<VkFormat> depthFormatList{ VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };
    for (VkFormat& format : depthFormatList) {
        VkFormatProperties2 formatProperties{ .sType = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2 };
        vkGetPhysicalDeviceFormatProperties2(parent.GetPhysicalDevice(), format, &formatProperties);
        if (formatProperties.formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
            depthFormat = format;
            break;
        }
    }
/*
    VkImageCreateInfo depthImageCreateInfo{
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = depthFormat,
        .extent{.width = swapChainExtent.width, .height = swapChainExtent.height, .depth = 1 },
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };

    VmaAllocationCreateInfo allocationCreateInfo{
        .flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO
    };

    vmaCreateImage(
        device.GetAllocator(),
        &depthImageCreateInfo,
        &allocationCreateInfo,
        &depthImage,
        &depthImageAllocation,
        nullptr
    );
*/

    spdlog::info("SwapChain Created Successfully");
    return true;
}

bool VulkanSwapchain::CreateImageViews()
{
    bool imageViewsInitialized = true;

    swapChainImageViews.resize(swapChainImages.size());
    for (uint32 i = 0; i < swapChainImages.size(); i++)
    {
        VkImageViewCreateInfo imageViewCreateInfo{
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = swapChainImages[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = imageFormat,
            .components {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY
            },
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        };

        if (vkCreateImageView(parent.GetLogicalDevice(), &imageViewCreateInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
            spdlog::error("Failed to create Image Views!");
            imageViewsInitialized = false;
            break;
        }
    }

    // VkImageViewCreateInfo depthViewCreateInfo {
    //     .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    //     .image = depthImage,
    //     .viewType = VK_IMAGE_VIEW_TYPE_2D,
    //     .format = depthFormat,
    //     .subresourceRange{ .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT, .levelCount = 1, .layerCount = 1 }
    // };
    //
    // //TODO: check if its created
    // vkCreateImageView(parent.GetLogicalDevice(), &depthViewCreateInfo, nullptr, &depthImageView);

    return imageViewsInitialized;
}