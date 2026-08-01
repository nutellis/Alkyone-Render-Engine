//
// Created by nutellis on 29/07/2026.
//

#ifndef ALKYONERENDERENGINE_VULKANFRAMEMANAGER_H
#define ALKYONERENDERENGINE_VULKANFRAMEMANAGER_H

#include <array>

#include "rhi/core/IFrameContext.h"
#include "rhi/core/managers/RHIFrameManager.h"
#include "rhi/vulkan/VulkanFrameContext.h"

class VulkanFrameManager : public RHIFrameManager{

public:
    VulkanFrameManager(VulkanDevice & device, VulkanSwapchain & swapchain);

    ~VulkanFrameManager() override;

    bool Initialize() override;
    void Terminate() override;

    bool BeginSynchronize() override;
    void SubmitQueue() override;
    void Present() override;
    void ResetSyncObjects() override;
    void EndSynchronize() override;

   [[nodiscard]] VkCommandBuffer GetCurrentCommandBuffer() const;

private:
    VulkanDevice & device;
    VulkanSwapchain & swapchain;

    std::array<VulkanFrameContext, FRAMES_IN_FLIGHT> frames;
};


#endif //ALKYONERENDERENGINE_VULKANFRAMEMANAGER_H