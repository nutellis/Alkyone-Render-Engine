//
// Created by Nutellis on 20-Jan-26.
//

#ifndef ALKYONERENDERENGINE_VULKANRHI_H
#define ALKYONERENDERENGINE_VULKANRHI_H
#include <unordered_map>

#include <rhi/core/DynamicRHI.h>

#include <volk.h>

#include <vector>

#include "containers/SlotMap.h"
#include "math/Vector3.h"

class VulkanBuffer;
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

    //Buffers
    Handle CreateBuffer(BufferDesc desc) override;
    IBuffer* GetBuffer(Handle handle) override;
    CopyRequest RecordCopyBuffer(Handle src, Handle dst, uint64 size) override;
    void SubmitCopyBuffer(std::vector<CopyRequest> copyRequests) override;
    void DestroyBuffer(Handle handle) override;

    //Drawing
    bool BeginFrame() override;
    void EndFrame() override;
    void ClearColour(Float3 colour) override;
    void BeginRendering() override;
    void EndRendering() override;
    void BindPipeline(uint32_t pipelineID) override;
    void PrepareVertexBuffer(Handle meshHandle);
    // void PrepareVertexBuffer(Mesh& mesh) override;

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
    std::vector<VulkanGraphicsPipeline> pipelineStorage;
    std::unordered_map<size_t, uint32> pipelineCache;

    //command buffer used at the moment
    VulkanCommandBuffer * cmd = nullptr;

    SlotMap<VulkanBuffer> buffers;
};



#endif //ALKYONERENDERENGINE_VULKANRHI_H
