//
// Created by Nutellis on 20-Jan-26.
//

#ifndef ALKYONERENDERENGINE_VULKANRHI_H
#define ALKYONERENDERENGINE_VULKANRHI_H
#include <unordered_map>

#include <rhi/core/DynamicRHI.h>

#include <volk.h>

#include <vector>

#include "VulkanDevice.h"
#include "VulkanSurface.h"
#include "VulkanSwapchain.h"
#include "math/Vector3.h"

class VulkanFrameSync;
class VulkanCommandBuffer;
class VulkanDevice;
class VulkanSwapchain;
class ARWindow;
class VulkanGraphicsPipeline;


class VulkanRHI: public DynamicRHI
{
public:

    VulkanRHI(ARWindow* windowHandle);

    bool Initialize() override;
    void Terminate() override;
    void SwapBuffers() override;

    std::string GetBackendString() override;
    uint32_t CreatePipeline(const GraphicsPipelineDesc& desc) override;
    ContextSlangTargetOptions GetSlangTargetOptions() override;

    bool CreateInstance();

    VkInstance GetInstance() const;

    void Validate() override;
    //Drawing
    bool BeginFrame() override;
    void EndFrame() override;
    void ClearColour(Float3 colour) override;
    void BeginRendering() override;
    void EndRendering() override;
    void BindPipeline(uint32_t pipelineID) override;
    void PrepareVertexBuffer(Mesh& mesh) override;

    void PrepareVertexBuffer(uint32_t bufferID) override;
    void BindIndexBuffer(uint32_t bufferID) override;
    void Draw() override;
    void WaitIdle() override;

    void TransitionBarrier(const ImageBarrier& barrier) override;

private:
    static std::vector<const char*> GetVulkanRequiredExtensions();

private:
    VkInstance instance = VK_NULL_HANDLE;

    VulkanDevice * device = nullptr;

    VulkanSwapchain * swapchain = nullptr;

    ARWindow * window = nullptr;

    VulkanFrameSync* frameSync = nullptr;

    //pipelines
    std::vector<VulkanGraphicsPipeline *> pipelineStorage;
    std::unordered_map<size_t, uint32> pipelineCache;

    //command buffer used at the moment
    VulkanCommandBuffer * cmd = nullptr;
};



#endif //ALKYONERENDERENGINE_VULKANRHI_H
