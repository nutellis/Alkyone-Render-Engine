//
// Created by Nutellis on 20-Jan-26.
//

#include "../../../include/rhi/vulkan/VulkanContext.h"
#include <GLFW/glfw3.h>

#define VOLK_IMPLEMENTATION
#define VMA_IMPLEMENTATION
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
#include <set>
#include <vk_mem_alloc.h>

#include "rhi/vulkan/VulkanGraphicsPipeline.h"
#include "spdlog/spdlog.h"

namespace
{
    bool CheckValidationLayerSupport()
    {
        uint32 layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);

        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                spdlog::error("Validation Layers not available");
                return false;
            }
        }

        return true;

    }

    uint32 FindQueueFamily(VkPhysicalDevice device)
    {
        uint32 queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int queueFamily = 0;
        for (int i = 0; i < queueFamilies.size(); i++)
        {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                queueFamily = i;
                break;
            }
        }

        return queueFamily;
    }

    bool CheckDeviceExtensionSupport(VkPhysicalDevice device) {

        uint32 extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
        uint32 formatCount, presentModeCount;

        SwapChainSupportDetails details {};

        //capabilities
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        //format
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
        }

        //present mode
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        bool extensionSupported = CheckDeviceExtensionSupport(device);
        bool swapChainSupported = false;

        if (extensionSupported)
        {
            SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device, surface);
            swapChainSupported = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
        && deviceFeatures.geometryShader
        && extensionSupported
        && swapChainSupported;

    }

    std::vector<const char*> getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        auto vulkanExtensions = std::vector<const char*>(glfwExtensions, glfwExtensions + glfwExtensionCount);

        // if (enableValidationLayers) {
        //     extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        // }

        return vulkanExtensions;
    }

    // https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#:~:text=FNV%2D1a%20hash%5Bedit%5D
    inline size_t hashPipeline(const GraphicsPipelineDesc& desc)
    {
        size_t hashBasis = 0xcbf29ce484222325;

        const uint8* data = reinterpret_cast<const uint8*>(&desc);

        for (size_t i = 0; i < sizeof(GraphicsPipelineDesc); ++i)
        {
            hashBasis = hashBasis ^ data[i];
            hashBasis = hashBasis * 1099511628211;
        }

        return hashBasis;
    }
}


VulkanContext::VulkanContext()
{

}

VulkanContext::VulkanContext(GLFWwindow* window) : window(window)
{
}

VulkanContext::~VulkanContext()
{

}

bool VulkanContext::Initialize()
{
    spdlog::info("Alkyone RHI: Initializing VulkanContext");
    if (volkInitialize() != VK_SUCCESS)
    {
        spdlog::error("Alkyone RHI: Critical failure, Volk Failed to Initialize");
        return false;
    }

    if (enableValidationLayers && !CheckValidationLayerSupport())
    {
        spdlog::error("Alkyone RHI: Critical failure during Vulkan initialization");
        return false;
    }

    if (CreateInstance()
        && CreateSurface()
        && PickPhysicalDevice()
        && CreateLogicalDevice()
        && CreateVMAAllocator()
        && CreateSwapChain()
        && CreateImageViews() == false)
    {
        spdlog::error("Alkyone RHI: Critical failure during Vulkan initialization");
        Terminate();
        return false;
    }

    backend = RendererBackend::Vulkan;
    slangTargetOptions = ContextSlangTargetOptions(
        SLANG_SPIRV,
        "spirv_1_4",
        slang::CompilerOptionName::EmitSpirvDirectly,
        {slang::CompilerOptionValueKind::Int, 1}
    );

    spdlog::info("Alkyone RHI: Context Initialized Successfully");
    return true;
}

void VulkanContext::Terminate()
{

    if (logicalDevice != VK_NULL_HANDLE) {
        for (auto imageView : swapChainImageViews) {
            vkDestroyImageView(logicalDevice, imageView, nullptr);
        }
        vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);
        vkDestroyDevice(logicalDevice, nullptr);
    }
    if (surface != VK_NULL_HANDLE)
    {
        vkDestroySurfaceKHR(instance, surface, nullptr);
    }
    if (instance != VK_NULL_HANDLE) {
        vkDestroyInstance(instance, nullptr);
    }
}

void VulkanContext::SwapBuffers()
{
}

std::string VulkanContext::GetBackendString()
{
    //silly check but fuck it, you never now.
    if (backend == RendererBackend::Vulkan)
    {
        return "Vulkan";
    }

    return "Backend Error";
}

uint32 VulkanContext::CreateGraphicsPipeline(const GraphicsPipelineDesc& desc)
{
    size_t hash = hashPipeline(desc);

    //check if pipeline exists
    if (pipelineCache.contains(hash))
    {
        return pipelineCache[hash];
    }

    VulkanGraphicsPipeline pipeline = VulkanGraphicsPipeline();
    pipeline.CreatePipeline(desc, logicalDevice);

    pipelineStorage.push_back(pipeline);
    uint32 index = pipelineStorage.size();

    pipelineCache[hash] = index;

    return index;
}

bool VulkanContext::CreateInstance()
{
    VkApplicationInfo appInfo{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Renderer",
        .applicationVersion = VK_MAKE_VERSION(1,0,0),
        .pEngineName = "Alkyone Render Engine",
        .engineVersion = VK_MAKE_VERSION(1,0,0),
        .apiVersion = VK_API_VERSION_1_3
    };

    // get extensions from glfw
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    auto extensions = getRequiredExtensions();

    if (extensions.empty())
    {
        spdlog::error("GLFW failed to provide required Vulkan instance extensions");
        return false;
    }

    VkInstanceCreateInfo createInfo{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &appInfo,
        .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
        .ppEnabledExtensionNames = extensions.data()
    };

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        spdlog::error("VkInstance failed to initiate");
        return false;
    }

    volkLoadInstance(instance);

    spdlog::info("Vulkan Instance Initialized Successfully");
    return true;
}

bool VulkanContext::CreateSurface()
{
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        spdlog::error("Failed to create surface");
        return false;
    }

    return true;
}

bool VulkanContext::PickPhysicalDevice()
{
    uint32 deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0)
    {
        spdlog::error("Failed to find GPUs with Vulkan support");
        return false;
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    // we need to check each device if it's suitable for the operation we want to perform
    for (const auto& device : devices) {
        if (IsDeviceSuitable(device, surface))
        {
            physicalDevice = device;

            VkPhysicalDeviceProperties2 deviceProperties{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2 };
            vkGetPhysicalDeviceProperties2(device, &deviceProperties);

            spdlog::info("Selected device: {}", deviceProperties.properties.deviceName);
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE)
    {
        spdlog::error("Failed to find a suitable GPU");
        return false;
    }

    return true;
}

bool VulkanContext::CreateLogicalDevice()
{
    uint32 indexFamily = FindQueueFamily(physicalDevice);
    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueCreateInfo{
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = indexFamily,
        .queueCount = 1,
        .pQueuePriorities = &queuePriority
    };

    VkPhysicalDeviceVulkan12Features enabledVk12Features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
        .descriptorIndexing = true,
        .descriptorBindingVariableDescriptorCount = true,
        .runtimeDescriptorArray = true,
        .bufferDeviceAddress = true
    };
    VkPhysicalDeviceVulkan13Features enabledVk13Features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
        .pNext = &enabledVk12Features,
        .synchronization2 = true,
        .dynamicRendering = true
    };

    const VkPhysicalDeviceFeatures enabledVk10Features{ .samplerAnisotropy = VK_TRUE };

    VkDeviceCreateInfo createInfo{
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = &enabledVk13Features,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &queueCreateInfo,
        .enabledLayerCount = 0,
        .enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()),
        .ppEnabledExtensionNames = deviceExtensions.data(),
        .pEnabledFeatures = &enabledVk10Features
    };

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS)
    {
        spdlog::error("Failed to create logical device");
        return false;
    }

    vkGetDeviceQueue(logicalDevice, indexFamily, 0, &graphicsQueue);

    spdlog::info("Vulkan Logical Device Initialized Successfully");
    return true;
}

bool VulkanContext::CreateVMAAllocator()
{
    VmaVulkanFunctions vkFunctions{
        .vkGetInstanceProcAddr = vkGetInstanceProcAddr,
        .vkGetDeviceProcAddr = vkGetDeviceProcAddr,
        .vkCreateImage = vkCreateImage
    };

    VmaAllocatorCreateInfo allocatorCreateInfo{
        .flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
        .physicalDevice = physicalDevice,
        .device = logicalDevice,
        .pVulkanFunctions = &vkFunctions,
        .instance = instance
    };

    if (vmaCreateAllocator(&allocatorCreateInfo, &allocator) != VK_SUCCESS)
    {
        spdlog::error("Failed to create VMA allocator");
        return false;
    }

    spdlog::info("Successfully created VMA allocator");
    return  true;
}

bool VulkanContext::CreateSwapChain()
{
    SwapChainSupportDetails swapChainSupportDetails = QuerySwapChainSupport(physicalDevice, surface);

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
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = VK_PRESENT_MODE_FIFO_KHR
    };

    if (vkCreateSwapchainKHR(logicalDevice, &swapChainCreateInfo, nullptr, &swapChain) != VK_SUCCESS)
    {
        spdlog::error("Failed to create swap chain");
        return false;
    }


    uint32 imageCount = 0;

    vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, swapChainImages.data());

    //TODO: check if its created
    std::vector<VkFormat> depthFormatList{ VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };
    for (VkFormat& format : depthFormatList) {
        VkFormatProperties2 formatProperties{ .sType = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2 };
        vkGetPhysicalDeviceFormatProperties2(physicalDevice, format, &formatProperties);
        if (formatProperties.formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
            depthFormat = format;
            break;
        }
    }

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
        allocator,
        &depthImageCreateInfo,
        &allocationCreateInfo,
        &depthImage,
        &depthImageAllocation,
        nullptr
    );

    return true;
}

bool VulkanContext::CreateImageViews()
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

        if (vkCreateImageView(logicalDevice, &imageViewCreateInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
            spdlog::error("failed to create image views!");
            imageViewsInitialized = false;
            break;
        }
    }

    VkImageViewCreateInfo depthViewCreateInfo {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = depthImage,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = depthFormat,
        .subresourceRange{ .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT, .levelCount = 1, .layerCount = 1 }
    };

    //TODO: check if its created
    vkCreateImageView(logicalDevice, &depthViewCreateInfo, nullptr, &depthImageView);

    return imageViewsInitialized;
}
