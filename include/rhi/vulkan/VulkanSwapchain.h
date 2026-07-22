//
// Created by Nutellis on 13-Feb-26.
//

#ifndef ALKYONERENDERENGINE_VULKANSWAPCHAIN_H
#define ALKYONERENDERENGINE_VULKANSWAPCHAIN_H
#include <vector>

#include "rhi/core/ISwapchain.h"

#include <volk.h>

#include "VulkanDevice.h"

class VulkanDevice;
class ARWindow;

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class VulkanSwapchain: public ISwapChain
{
public:
    VulkanSwapchain(VulkanDevice& inParent, ARWindow& inWindow);
    ~VulkanSwapchain() override;

    bool Initialize() override;
    void Terminate() override;
    void RecreateSwapChain() override;
    void CleanupSwapChain() override;
    void Present(IFrameSync& frameSync) override;

    [[nodiscard]] bool IsFrameBufferResized() const override;
    void ResetIsFrameBufferResized() const override;


    bool CheckSwapChainSupport();

    VkSurfaceKHR GetVkSurface() const;
    VkSwapchainKHR GetVkSwapchain() const;

    VkImage GetVkImage(uint32_t index) const;
    VkImageView GetVkImageView(uint32_t index) const;
    VkExtent2D GetVkExtent() const;



private:
    bool CreateSwapChain();
    bool CreateImageViews();

private:

    VkInstance instance = VK_NULL_HANDLE;
    VulkanDevice & parent;

    ARWindow & window;

    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkSwapchainKHR swapChain = VK_NULL_HANDLE;

    SwapChainSupportDetails swapChainSupportDetails;

    std::vector<VkImage> swapChainImages;
    VkImage depthImage {};

    VkFormat imageFormat {};
    VkFormat depthFormat {};
    VkExtent2D swapChainExtent {};

    std::vector<VkImageView> swapChainImageViews;
    VkImageView depthImageView {};

};


#endif //ALKYONERENDERENGINE_VULKANSWAPCHAIN_H