//
// Created by Nutellis on 20-Jan-26.
//

#ifndef ALKYONERENDERENGINE_DYNAMICRHI_H
#define ALKYONERENDERENGINE_DYNAMICRHI_H

#include <slang-com-ptr.h>
#include <string>

#include "RHIBuffer.h"
#include "ICommandBuffer.h"
#include "slang.h"
#include "containers/SlotMap.h"
#include "math/Vector3.h"
#include "shader/RHIShader.h"
#include "resources/ResourceHandle.h"

class ARWindow;
class IFrameSync;

struct ShaderCompileDesc;
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

    virtual void WaitIdle() = 0;

    static DynamicRHI* CreateContext(ARWindow* windowHandle, RendererBackend rendererBackend);

    virtual void Validate() = 0;

    //Shaders
    /*
        Note
        Currently, the global session type is not thread-safe.
        Applications that wish to compile on multiple threads will need to ensure that each concurrent thread compiles with a distinct global session.

        Note
        Currently, the global session should be freed after any objects created from it.
    */
    virtual bool InitializeSlang(const ContextSlangTargetOptions& slangInitOptions) = 0;
    virtual ShaderHandle CreateShader(const ShaderCompileDesc& desc) = 0;
    virtual void DestroyShader(ShaderHandle handle) = 0;
    virtual RHIShader * GetShader(ShaderHandle ShaderHandle) = 0;

    // Buffers
    virtual BufferHandle CreateBuffer(const BufferDesc & desc) = 0;
    virtual RHIBuffer &GetBuffer(BufferHandle bufferHandle) = 0;
    virtual void HostCopyBuffer(BufferHandle bufferHandle, const void * src, size_t size, size_t offset) = 0;
    virtual CopyRequest RecordCopyBuffer(BufferHandle src, BufferHandle dst, uint64 size, size_t srcOffset, size_t dstOffset) = 0;
    virtual void SubmitCopyBuffer(std::vector<CopyRequest> copyRequests) = 0;
    virtual void DestroyBuffer(BufferHandle bufferHandle) = 0;

    //Pipelines
    virtual PipelineHandle CreatePipeline(const GraphicsPipelineDesc & desc) = 0;
    virtual void DestroyPipeline(PipelineHandle pipelineHandle) = 0;

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

    Slang::ComPtr<slang::IGlobalSession> globalSession;
    Slang::ComPtr<slang::ISession> slangSession;
    SlangGlobalSessionDesc desc = {};

};


#endif //ALKYONERENDERENGINE_DYNAMICRHI_H