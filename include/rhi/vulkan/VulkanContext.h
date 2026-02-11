//
// Created by Nutellis on 20-Jan-26.
//

#ifndef ALKYONERENDERENGINE_VULKANCONTEXT_H
#define ALKYONERENDERENGINE_VULKANCONTEXT_H
#include <unordered_map>

#include <rhi/IGraphicsContext.h>

#include <volk.h>
#include <vk_mem_alloc.h>

#include <vector>
#include <core/PODTypes.h>

#include "containers/SlotMap.h"
#include "rhi/IGraphicsPipeline.h"

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

const std::vector validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

class ARWindow;
class VulkanGraphicsPipeline;

using PipelineId = SlotMap<VkPipeline>::ElementId;

struct QueueFamilyIndices {
    uint32 graphicsFamily;
};

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class VulkanContext: public IGraphicsContext
{
    VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice logicalDevice = VK_NULL_HANDLE;
    VkQueue graphicsQueue = VK_NULL_HANDLE;

    VmaAllocator allocator{ VK_NULL_HANDLE };
    VmaAllocation depthImageAllocation;

    VkSurfaceKHR surface = VK_NULL_HANDLE;

    VkSwapchainKHR swapChain = VK_NULL_HANDLE;

    std::vector<VkImage> swapChainImages;
    VkImage depthImage;

    VkFormat imageFormat;
    VkFormat depthFormat;
    VkExtent2D swapChainExtent;

    std::vector<VkImageView> swapChainImageViews;
    VkImageView depthImageView;

    GLFWwindow * window = nullptr;

    std::vector<VulkanGraphicsPipeline> pipelineStorage;
    std::unordered_map<size_t, size_t> pipelineCache;

public:

    VulkanContext();
    VulkanContext(GLFWwindow * window);
    ~VulkanContext();

    bool Initialize() override;
    void Terminate() override;
    void SwapBuffers() override;
    std::string GetBackendString() override;
    uint32_t CreateGraphicsPipeline(const GraphicsPipelineDesc& desc) override;

private:
    bool CreateInstance();
    bool CreateSurface();
    bool PickPhysicalDevice();
    bool CreateLogicalDevice();
    bool CreateVMAAllocator();
    bool CreateSwapChain();
    bool CreateImageViews();

};



#endif //ALKYONERENDERENGINE_VULKANCONTEXT_H
