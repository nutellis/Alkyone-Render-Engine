//
// Created by Nutellis on 20-Jan-26.
//

#ifndef ALKYONERENDERENGINE_DYNAMICRHI_H
#define ALKYONERENDERENGINE_DYNAMICRHI_H

#include <string>

#include "ICommandBuffer.h"
#include "slang.h"
#include "math/Vector3.h"

class ARWindow;
class IBuffer;
class IFrameSync;


struct CommandPoolDesc;
struct GraphicsPipelineDesc;
struct ImageBarrier;
struct BufferDesc;
struct Handle;

enum class RendererBackend {
    OpenGL,
    Vulkan,
    D3D12
};

struct ContextSlangTargetOptions
{
    SlangCompileTarget format;
    std::string profile;
    slang::CompilerOptionName name;
    slang::CompilerOptionValue value;

};

class DynamicRHI
{
public:
    virtual ~DynamicRHI() = default;

    virtual bool Initialize() = 0;
    virtual void Terminate() = 0;
    virtual void SwapBuffers() = 0;

    virtual std::string GetBackendString() = 0;
    virtual ContextSlangTargetOptions GetSlangTargetOptions() = 0;

    virtual uint32 CreatePipeline(const GraphicsPipelineDesc& desc) = 0;
    virtual void WaitIdle() = 0;

    static DynamicRHI* CreateContext(ARWindow* windowHandle, RendererBackend rendererBackend);

    virtual void Validate() = 0;

    // Buffers
    virtual Handle CreateBuffer(BufferDesc desc) = 0;
    virtual IBuffer* GetBuffer(Handle handle) = 0;
    virtual CopyRequest RecordCopyBuffer(Handle src, Handle dst, uint64 size) = 0;
    virtual void SubmitCopyBuffer(std::vector<CopyRequest> copyRequests) = 0;
    virtual void DestroyBuffer(Handle buffer) = 0;

    // Drawing
    virtual bool BeginFrame() = 0;
    virtual void EndFrame() = 0;
    virtual void ClearColour(Float3 colour) = 0;
    virtual void BeginRendering() = 0;
    virtual void EndRendering() = 0;
    virtual void BindPipeline(uint32_t pipelineID) = 0;
    virtual void PrepareVertexBuffer(uint32_t bufferID) = 0;
    //virtual void PrepareVertexBuffer(Mesh& mesh) = 0;
    virtual void BindIndexBuffer(uint32_t bufferID) = 0;
    //TODO: find out what is needed.
    //virtual void DrawIndexed() = 0;
    virtual void Draw() = 0;

    virtual void TransitionBarrier(const ImageBarrier& barrier) = 0;

protected:
    RendererBackend backend {};
    ContextSlangTargetOptions slangTargetOptions {};
};


#endif //ALKYONERENDERENGINE_DYNAMICRHI_H