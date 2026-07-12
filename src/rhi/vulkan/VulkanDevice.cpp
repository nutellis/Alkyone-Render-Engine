//
// Created by Nutellis on 13-Feb-26.
//

#include <vector>
#include <rhi/vulkan/VulkanDevice.h>

#define VMA_IMPLEMENTATION
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
#include <vk_mem_alloc.h>


#include <set>
#include "rhi/vulkan/VulkanQueue.h"
#include "rhi/vulkan/VulkanRHI.h"
#include "spdlog/spdlog.h"

VulkanDevice::VulkanDevice(VkInstance instance) : instance(instance)
{
}

VulkanDevice::~VulkanDevice()
{
    for (VulkanQueue* queue : queues)
    {
        delete queue;
        queue = nullptr;
    }
}

bool VulkanDevice::Initialize()
{
    if (enableValidationLayers && !CheckValidationLayerSupport())
    {
        spdlog::error("Alkyone RHI: Critical failure during Vulkan initialization");
        return false;
    }

    if (PickPhysicalDevice(instance)
        && CreateLogicalDevice()
        && CreateVMAAllocator(instance) == false)
    {
        spdlog::error("Alkyone RHI: Critical failure during Vulkan Device initialization");
        return false;
    }

    return true;
}

void VulkanDevice::Terminate()
{
    for (VulkanQueue* queue : queues)
    {
        queue->Terminate();
    }

    if (allocator != VK_NULL_HANDLE) {
        vmaDestroyAllocator(allocator);
    }

    if (logicalDevice != VK_NULL_HANDLE) {
        vkDestroyDevice(logicalDevice, nullptr);
    }
}

std::vector<VkDeviceQueueCreateInfo> VulkanDevice::PopulateQueueFamilies()
{
    uint32 queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    for (uint32 i = 0; i < queueFamilyCount; i++)
    {
        const auto& flags = queueFamilies[i].queueFlags;
        if ((flags & VK_QUEUE_GRAPHICS_BIT) && graphicsIndex == UINT32_MAX)
        {
            graphicsIndex = i;
        }
        // If the device has dedicated compute queue.
        if ((flags & VK_QUEUE_COMPUTE_BIT)
            && !(flags & VK_QUEUE_GRAPHICS_BIT)
            && computeIndex == UINT32_MAX)
        {
            computeIndex = i;
        }
        // If the device has dedicated transfer queue.
        if ((flags & VK_QUEUE_TRANSFER_BIT)
            && !(flags & VK_QUEUE_GRAPHICS_BIT)
            && !(flags & VK_QUEUE_COMPUTE_BIT)
            && transferIndex == UINT32_MAX)
        {
            transferIndex = i;
        }
    }

    // //If no dedicated compute/transfer queue found, fallback to graphics
    // if (computeIndex == UINT32_MAX)
    // {
    //     computeIndex = graphicsIndex;
    // }
    // if (transferIndex == UINT32_MAX)
    // {
    //     transferIndex = graphicsIndex;
    // }

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32> uniqueFamilies = { graphicsIndex, computeIndex, transferIndex };
    float priority = 1.0f;
    for (uint32 idx : uniqueFamilies) {
        VkDeviceQueueCreateInfo queueInfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
        queueInfo.queueFamilyIndex = idx;
        queueInfo.queueCount = 1;
        queueInfo.pQueuePriorities = &priority;
        queueCreateInfos.push_back(queueInfo);
    }

    return queueCreateInfos;
}



VkPhysicalDevice VulkanDevice::GetPhysicalDevice() const
{
    return physicalDevice;
}

VkDevice VulkanDevice::GetLogicalDevice() const
{
    return logicalDevice;
}

VmaAllocator VulkanDevice::GetAllocator() const
{
    return allocator;
}

VulkanQueue* VulkanDevice::GetGraphicsQueue() const
{
    return queues[GRAPHICS_QUEUE];
}

VulkanQueue* VulkanDevice::GetTransferQueue() const
{
    return queues[TRANSFER_QUEUE];
}

VkInstance VulkanDevice::GetInstance() const
{
    return instance;
}

void VulkanDevice::WaitIdle()
{
    vkDeviceWaitIdle(logicalDevice);
}

bool VulkanDevice::PickPhysicalDevice(VkInstance instance)
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
        if (IsDeviceSuitable(device))
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

bool VulkanDevice::CreateLogicalDevice()
{
    auto queueFamilies = PopulateQueueFamilies();

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
        .queueCreateInfoCount = static_cast<uint32>(queueFamilies.size()),
        .pQueueCreateInfos = queueFamilies.data(),
        .enabledLayerCount = 0,
        .enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()),
        .ppEnabledExtensionNames = deviceExtensions.data(),
        .pEnabledFeatures = &enabledVk10Features
    };

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS)
    {
        spdlog::error("Failed to create Logical Device");
        return false;
    }

    volkLoadDevice(logicalDevice);

    if (CreateQueues() == false)
    {
        spdlog::error("Failed to create Logical Device");
        return false;
    }

    spdlog::info("Vulkan Logical Device Initialized Successfully");
    return true;
}

bool VulkanDevice::CreateQueues()
{
    bool queuesInitialized = true;
    // Queue initialization
    queues[GRAPHICS_QUEUE] = new VulkanQueue(*this, graphicsIndex, GRAPHICS_QUEUE);
    queuesInitialized = queues[GRAPHICS_QUEUE]->Initialize(FRAMES_IN_FLIGHT);
    // no dedicated queue found. We leave the index nullptr;
    if (computeIndex == UINT32_MAX)
    {
        spdlog::warn("No dedicated compute queue found");
        queues[COMPUTE_QUEUE] = nullptr;
    } else
    {
        queues[COMPUTE_QUEUE] = new VulkanQueue(*this, computeIndex, COMPUTE_QUEUE);
        queuesInitialized &= queues[COMPUTE_QUEUE]->Initialize(FRAMES_IN_FLIGHT);
    }
    if (transferIndex == UINT32_MAX)
    {
        spdlog::warn("No dedicated transfer queue found");
        queues[TRANSFER_QUEUE] = nullptr;
    } else
    {
        queues[TRANSFER_QUEUE] = new VulkanQueue(*this, transferIndex, TRANSFER_QUEUE);
        queuesInitialized &= queues[TRANSFER_QUEUE]->Initialize(1);
    }

    if (queuesInitialized == false)
    {
        spdlog::error("Failed to initialize queues");
        return false;
    }

    spdlog::info("Vulkan Queues Initialized Successfully");
    return true;
}

bool VulkanDevice::CreateVMAAllocator(VkInstance instance)
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

bool VulkanDevice::CheckValidationLayerSupport()
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


bool VulkanDevice::IsDeviceSuitable(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    bool extensionSupported = CheckDeviceExtensionSupport(device);

    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
    && deviceFeatures.geometryShader
    && extensionSupported;

}

bool VulkanDevice::CheckDeviceExtensionSupport(VkPhysicalDevice device) {

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

