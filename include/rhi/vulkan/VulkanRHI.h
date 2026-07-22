//
// Created by Nutellis on 20-Jan-26.
//

#ifndef ALKYONERENDERENGINE_VULKANRHI_H
#define ALKYONERENDERENGINE_VULKANRHI_H
#include <unordered_map>

#include <rhi/core/DynamicRHI.h>

#include <volk.h>

#include <vector>

#include "vk_mem_alloc.h"
#include "containers/SlotMap.h"
#include "math/Vector3.h"
#include "resources/ResourceHandle.h"

struct VulkanShader;
struct VulkanBuffer;
struct VulkanGraphicsPipeline;
class VulkanFrameSync;
class VulkanCommandBuffer;
class VulkanDevice;
class VulkanSwapchain;
class ARWindow;


enum class InitResult
{
    Success,
    VolkInitializationFailed,
    InstanceCreationFailed,
    DeviceInitializationFailed,
    SwapchainInitializationFailed,
    FrameSyncInitializationFailed,
    SlangInitializationFailed
};

class VulkanRHI: public DynamicRHI
{
public:

    VulkanRHI(ARWindow* windowHandle);

    bool Initialize() override;
    void Terminate() override;
    void SwapBuffers() override;

    std::string GetBackendString() override;
    ContextSlangTargetOptions GetSlangTargetOptions() override;

    bool CreateInstance();

    VkInstance GetInstance() const;

    void Validate() override;


    //shaders
    bool InitializeSlang(const ContextSlangTargetOptions &slangInitOptions) override;
    ShaderHandle CreateShader(const ShaderCompileDesc &desc) override;

    void DestroyShader(ShaderHandle handle) override;
    void DestroyShader(VulkanShader &shader, VkDevice logicalDevice);

    RHIShader * GetShader(ShaderHandle ShaderHandle) override;


    //Buffers
    BufferHandle CreateBuffer(const BufferDesc & desc) override;

    RHIBuffer &GetBuffer(BufferHandle bufferHandle) override;
    void HostCopyBuffer(BufferHandle bufferHandle, const void * src, size_t size, size_t offset) override;
    CopyRequest RecordCopyBuffer(BufferHandle src, BufferHandle dst, uint64 size, size_t srcOffset, size_t dstOffset) override;
    void SubmitCopyBuffer(std::vector<CopyRequest> copyRequests) override;

    void DestroyBuffer(BufferHandle bufferHandle) override;
    void DestroyBuffer(VulkanBuffer &vulkanBuffer, VmaAllocator allocator);

    //Pipelines
    PipelineHandle CreatePipeline(const GraphicsPipelineDesc & desc) override;
    void DestroyPipeline(PipelineHandle pipelineHandle) override;
    void DestroyPipeline(VulkanGraphicsPipeline& vulkanPipeline, VkDevice logicalDevice);

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
    std::unordered_map<size_t, uint32> pipelineCache;

    //command buffer used at the moment
    VulkanCommandBuffer * cmd = nullptr;

    std::vector<VulkanBuffer> buffers;
    std::vector<VulkanShader> shaders;
    std::vector<VulkanGraphicsPipeline> pipelines;
};



#endif //ALKYONERENDERENGINE_VULKANRHI_H
