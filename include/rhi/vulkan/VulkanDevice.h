//
// Created by Nutellis on 13-Feb-26.
//

#ifndef ALKYONERENDERENGINE_VULKANDEVICE_H
#define ALKYONERENDERENGINE_VULKANDEVICE_H

#include <array>

#include "rhi/core/IDevice.h"
#include <core/PODTypes.h>

#include <volk.h>

#include "vk_mem_alloc.h"
#include "VulkanRHI.h"

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

const std::vector validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

inline std::vector deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

class VulkanQueue;

class VulkanDevice: public IDevice
{
public:
    VulkanDevice(VkInstance instance);
    ~VulkanDevice() override;

    bool Initialize() override;
    void Terminate() override;

    [[nodiscard]] VkPhysicalDevice GetPhysicalDevice() const;
    [[nodiscard]] VkDevice GetLogicalDevice() const;
    [[nodiscard]] VmaAllocator GetAllocator() const;
    [[nodiscard]] VulkanQueue* GetGraphicsQueue() const;

    VkInstance GetInstance() const;
    void WaitIdle() override;

private:
    bool PickPhysicalDevice(VkInstance instance);
    bool CreateLogicalDevice();
    bool CreateQueues();
    bool CreateVMAAllocator(VkInstance instance);

    bool CheckValidationLayerSupport();
    std::vector<VkDeviceQueueCreateInfo> PopulateQueueFamilies();

    static bool IsDeviceSuitable(VkPhysicalDevice device);
    static bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

private:
    VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice logicalDevice = VK_NULL_HANDLE;

    VmaAllocator allocator{ VK_NULL_HANDLE };
    VmaAllocation depthImageAllocation{ VK_NULL_HANDLE };

    //one queue per type
    std::array<VulkanQueue*, 3> queues;

    uint32 graphicsIndex = UINT32_MAX;
    uint32 computeIndex = UINT32_MAX;
    uint32 transferIndex = UINT32_MAX;
};


#endif //ALKYONERENDERENGINE_VULKANDEVICE_H