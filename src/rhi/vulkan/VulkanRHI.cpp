//
// Created by Nutellis on 20-Jan-26.
//

#include <rhi/vulkan/VulkanRHI.h>

#define VOLK_IMPLEMENTATION

#include <core/ApplicationWindow.h>

#include <rhi/core/RHIGraphicsPipeline.h>
#include "rhi/vulkan/VulkanCommandPool.h"
#include "rhi/vulkan/VulkanFrameSync.h"

#include <set>

#include "rendering/types/Vertex.h"
#include "rhi/vulkan/VulkanBuffer.h"
#include "rhi/vulkan/VulkanCommandBuffer.h"
#include "rhi/vulkan/VulkanDefinitions.h"
#include "rhi/vulkan/VulkanGraphicsPipeline.h"
#include "rhi/vulkan/VulkanQueue.h"
#include "spdlog/spdlog.h"

#include <utilities/TypeUtilities.h>

#include "rhi/descriptors/GraphicsPipelineDesc.h"
#include "rhi/descriptors/ShaderCompileDesc.h"
#include "rhi/vulkan/VulkanBufferTransforms.h"
#include "rhi/vulkan/VulkanShader.h"
#include "rhi/vulkan/VulkanSwapchain.h"

namespace
{
    // https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#:~:text=FNV%2D1a%20hash%5Bedit%5D
    inline size_t hashPipeline(const GraphicsPipelineDesc& desc)
    {
        size_t hashBasis = 0xcbf29ce484222325;

        const uint8* data = reinterpret_cast<const uint8*>(&desc);

        for (size_t i = 0; i < sizeof(GraphicsPipelineDesc); ++i)
        {
            hashBasis = hashBasis ^ data[i];
            hashBasis = hashBasis * 1099511628211;
        }

        return hashBasis;
    }
}

VulkanRHI::VulkanRHI(ARWindow * windowHandle) : window(windowHandle)
{
}

bool VulkanRHI::Initialize()
{
    spdlog::info("Alkyone RHI: Initializing Vulkan Context");

    if (volkInitialize() != VK_SUCCESS)
    {
        spdlog::error("Alkyone RHI: Critical failure, Volk Failed to Initialize");
        return false;
        //return InitResult::VolkInitializationFailed;
    }

    if (CreateInstance() == false)
    {
        spdlog::error("Alkyone RHI: Critical failure, failed to initialize Vulkan Instance");
        return false;
    }

    device = new VulkanDevice(instance);
    swapchain = new VulkanSwapchain(*device, *window);
    frameSync = new VulkanFrameSync(*device, *swapchain);

    if ((device->Initialize()
        && swapchain->Initialize()
        && frameSync->Initialize())
        == false)
    {
        spdlog::error("Alkyone RHI: Critical failure during Vulkan initialization");
        Terminate();
        return false;
    }
    //allocate buffer space for each frame in flight.
    device->GetGraphicsQueue()->AllocateCommandBuffers(FRAMES_IN_FLIGHT);

    backend = RendererBackend::Vulkan;
    slangTargetOptions = ContextSlangTargetOptions(
        SLANG_SPIRV,
        "spirv_1_4",
        slang::CompilerOptionName::EmitSpirvDirectly,
        {slang::CompilerOptionValueKind::Int, 1}
    );
    // TODO: add a check block for slang
    InitializeSlang(slangTargetOptions);

    spdlog::info("Alkyone RHI: Context Initialized Successfully");
    return true;
}

void VulkanRHI::Terminate()
{
    VkDevice logicalDevice = device->GetLogicalDevice();
    VmaAllocator allocator = device->GetAllocator();

    vkDeviceWaitIdle(logicalDevice);

    for (auto& buffer : buffers) {
        DestroyBuffer(buffer,allocator);
    }
    buffers.clear();
    buffers.shrink_to_fit();

    for (auto& shader : shaders) {
       DestroyShader(shader, logicalDevice);
    }
    shaders.clear();
    shaders.shrink_to_fit();

    for (auto pipeline : pipelines)
    {
        DestroyPipeline(pipeline, logicalDevice);
    }
    pipelines.clear();
    pipelines.shrink_to_fit();

    frameSync->Terminate();
    swapchain->Terminate();
    device->Terminate();

    if (instance != VK_NULL_HANDLE) {
        vkDestroyInstance(instance, nullptr);
    }
}

void VulkanRHI::SwapBuffers()
{
}


// void VulkanRHI::DrawFrame(VulkanFrameSync* frame)
// {
//     //TODO: maybe use get_current_frame func instead
//     VulkanFrameSync * vFrame = static_cast<VulkanFrameSync *>(frame);
//     // wait until the gpu has finished rendering the last frame. Timeout of 1
//     // second

//     // create an image memory barrier to prepare the swapchain image.
//     VkImageMemoryBarrier2 imageBarrier {.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2};
//     imageBarrier.pNext = nullptr;
//
//     imageBarrier.srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
//     imageBarrier.srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT;
//     imageBarrier.dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
//     imageBarrier.dstAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT | VK_ACCESS_2_MEMORY_READ_BIT;
//
//     imageBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//     imageBarrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
//
//     VkImageAspectFlags aspectMask = (VK_IMAGE_LAYOUT_GENERAL == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
//     imageBarrier.subresourceRange = {
//         .aspectMask = aspectMask,
//         .baseMipLevel = 0,
//         .levelCount = VK_REMAINING_MIP_LEVELS,
//         .baseArrayLayer = 0,
//         .layerCount = VK_REMAINING_ARRAY_LAYERS
//     },
//     imageBarrier.image = swapchain->GetSwapchainImage(swapchainImageIndex);
//
//     //insert barrier onto the command buffer
//     VkDependencyInfo depInfo {};
//     depInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
//     depInfo.pNext = nullptr;
//
//     depInfo.imageMemoryBarrierCount = 1;
//     depInfo.pImageMemoryBarriers = &imageBarrier;
//
//     vkCmdPipelineBarrier2(cmd, &depInfo);
//
//     //make a clear-color from frame number. This will flash with a 120 frame period.
//     VkClearColorValue clearValue;
//     float flash = std::abs(std::sin(_frameNumber / 120.f));
//     clearValue = { { 0.0f, 0.0f, flash, 1.0f } };
//
//     VkImageSubresourceRange clearRange {};
//     clearRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//     clearRange.baseMipLevel = 0;
//     clearRange.levelCount = VK_REMAINING_MIP_LEVELS;
//     clearRange.baseArrayLayer = 0;
//     clearRange.layerCount = VK_REMAINING_ARRAY_LAYERS;
//
//     //clear image
//     vkCmdClearColorImage(cmd, swapchain->GetSwapchainImage(swapchainImageIndex), VK_IMAGE_LAYOUT_GENERAL, &clearValue, 1, &clearRange);
//
//
//     //make the swapchain image into presentable mode
//
//     VkImageMemoryBarrier2 imageBarrier2 {.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2};
//     imageBarrier2.pNext = nullptr;
//
//     imageBarrier2.srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
//     imageBarrier2.srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT;
//     imageBarrier2.dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
//     imageBarrier2.dstAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT | VK_ACCESS_2_MEMORY_READ_BIT;
//
//     imageBarrier2.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
//     imageBarrier2.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
//
//     VkImageAspectFlags aspectMask2 = (VK_IMAGE_LAYOUT_PRESENT_SRC_KHR == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
//     imageBarrier2.subresourceRange = {
//         .aspectMask = aspectMask2,
//         .baseMipLevel = 0,
//         .levelCount = VK_REMAINING_MIP_LEVELS,
//         .baseArrayLayer = 0,
//         .layerCount = VK_REMAINING_ARRAY_LAYERS
//     },
//     imageBarrier2.image = swapchain->GetSwapchainImage(swapchainImageIndex);
//
//     VkDependencyInfo depInfo2 {};
//     depInfo2.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
//     depInfo2.pNext = nullptr;
//
//     depInfo2.imageMemoryBarrierCount = 1;
//     depInfo2.pImageMemoryBarriers = &imageBarrier2;
//
//     vkCmdPipelineBarrier2(cmd, &depInfo2);
//
//     vkEndCommandBuffer(cmd);
//
// }

std::string VulkanRHI::GetBackendString()
{
    //silly check but fuck it, you never now.
    if (backend == RendererBackend::Vulkan)
    {
        return "Vulkan";
    }

    return "Backend Error";
}



// uint32 VulkanRHI::CreatePipeline(const GraphicsPipelineDesc& desc)
// {
//     size_t hash = hashPipeline(desc);
//
//     //check if pipeline exists
//     if (pipelineCache.contains(hash))
//     {
//         return pipelineCache[hash];
//     }
//
//     VulkanGraphicsPipeline pipeline = VulkanGraphicsPipeline();
//     pipeline.CreatePipeline(desc, logicalDevice);
//
//     pipelineStorage.push_back(pipeline);
//     size_t index = pipelineStorage.size();
//
//     pipelineCache[hash] = static_cast<uint32>(index);
//
//     return static_cast<uint32>(index);
// }

// bool VulkanRHI::CreateGraphicsCommandPool(const CommandPoolDesc& desc)
// {
//     //TODO: delete temp
//     CommandPoolDesc tempDesc = {
//         COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
//         FindQueueFamily(physicalDevice)
//     };
//
//     VulkanCommandPool pool = VulkanCommandPool();
//     if (!pool.CreateCommandPool(tempDesc, logicalDevice))
//     {
//         return false;
//     }
//
//     commandPools.push_back(pool);
//     return true;
// }
//

ContextSlangTargetOptions VulkanRHI::GetSlangTargetOptions()
{
    return slangTargetOptions;
}

bool VulkanRHI::CreateInstance()
{
    VkApplicationInfo appInfo{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Renderer",
        .applicationVersion = VK_MAKE_VERSION(1,0,0),
        .pEngineName = "Alkyone Render Engine",
        .engineVersion = VK_MAKE_VERSION(1,0,0),
        .apiVersion = VK_API_VERSION_1_3
    };

    auto extensions = GetVulkanRequiredExtensions();

    if (extensions.empty())
    {
        spdlog::error("GLFW failed to provide required Vulkan instance extensions");
        return false;
    }

    VkInstanceCreateInfo createInfo{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &appInfo,
        .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
        .ppEnabledExtensionNames = extensions.data()
    };

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        spdlog::error("VkInstance failed to initiate");
        return false;
    }

    volkLoadInstance(instance);

    spdlog::info("Vulkan Instance Initialized Successfully");
    return true;
}

VkInstance VulkanRHI::GetInstance() const
{
    return instance;
}

void VulkanRHI::Validate()
{
    //validate if all relevant data is present before doing anything.

    if (!frameSync && !swapchain && !device)
    {
        spdlog::error("VulkanRHI: Validation Failed, missing frameSync, swapchain or device");
        exit(EXIT_FAILURE);
    }
}

//-----------------------------------------------------------------------
// SHADERS
//-----------------------------------------------------------------------
bool VulkanRHI::InitializeSlang(const ContextSlangTargetOptions &slangInitOptions) {
    /*
    If you want to enable GLSL compatibility mode, you need to set SlangGlobalSessionDesc::enableGLSL to true when calling createGlobalSession().
    This will load the necessary GLSL intrinsic module for compiling GLSL code.
    Without this setting, compiling GLSL code will result in an error.
    */
    createGlobalSession(&desc, globalSession.writeRef());

    // create session
    auto slangTargets{
        std::to_array<slang::TargetDesc>({ {
            .format = slangInitOptions.format,
            .profile = globalSession->findProfile(slangInitOptions.profile.c_str()),
            }
        })
    };

    auto slangOptions{ std::to_array<slang::CompilerOptionEntry>({ {
        slangInitOptions.name,
        slangInitOptions.value
    } }) };

    const char * searchPaths[] = { "../shaders/" };

    slang::SessionDesc slangSessionDesc{
        .targets = slangTargets.data(),
        .targetCount = static_cast<SlangInt>(slangTargets.size()),
        .defaultMatrixLayoutMode = SLANG_MATRIX_LAYOUT_COLUMN_MAJOR,
        .searchPaths = searchPaths,
        .searchPathCount = 1,
        .compilerOptionEntries = slangOptions.data(),
        .compilerOptionEntryCount = static_cast<uint32>(slangOptions.size())

    };

    if (globalSession->createSession(slangSessionDesc, slangSession.writeRef()) != SLANG_OK)
    {
        spdlog::error("Slang failed to Create Session");
        return false;
    }

    spdlog::info("Slang Session Created");
    return true;
}

ShaderHandle VulkanRHI::CreateShader(const ShaderCompileDesc &desc) {

    Slang::ComPtr<slang::IBlob> diagnostics;

    //TODO: move this to its own class or at least function
    Slang::ComPtr<slang::IModule> slangModule {
        slangSession->loadModule(desc.filename.c_str(),diagnostics.writeRef())
    };
    if (slangModule == nullptr) {
        spdlog::error("Failed to load module {}: {}", desc.filename,
            diagnostics ? static_cast<const char *>(diagnostics->getBufferPointer()) : "Unknown error");
        return ShaderHandle{Handle::Invalid()};
    }

    slang::IEntryPoint * vertexEntryPoint = nullptr;
    slangModule->findEntryPointByName(desc.vertexEntryPoint.c_str(), &vertexEntryPoint);

    slang::IEntryPoint* fragmentEntryPoint = nullptr;
    slangModule->findEntryPointByName(desc.fragmentEntryPoint.c_str(), &fragmentEntryPoint);

    if (!vertexEntryPoint || !fragmentEntryPoint)
    {
        spdlog::error("Failed to find entry points in {}", desc.filename);
        return ShaderHandle{Handle::Invalid()};
    }

    slang::IComponentType * componentTypes[] = {slangModule, vertexEntryPoint, fragmentEntryPoint};
    Slang::ComPtr<slang::IComponentType> slangProgram;

    SlangResult result = slangSession->createCompositeComponentType(
        componentTypes,
        3,
        slangProgram.writeRef(),
        diagnostics.writeRef()
    );

    if (SLANG_FAILED(result)) {
        spdlog::error("Failed to link shader program: {}",
            diagnostics ? static_cast<const char *>(diagnostics->getBufferPointer()) : "Unknown error");
        return ShaderHandle{Handle::Invalid()};
    }

    Slang::ComPtr<slang::IBlob> spirvVertex;
    result = slangProgram->getEntryPointCode(
        0,
        0,
        spirvVertex.writeRef(),
        diagnostics.writeRef()
    );
    if (SLANG_FAILED(result))
    {
        spdlog::error("Failed to extract Vertex SPIR-V: {}",
            diagnostics ? static_cast<const char *>(diagnostics->getBufferPointer()) : "Unknown error");
        return ShaderHandle{Handle::Invalid()};
    }

    Slang::ComPtr<slang::IBlob> spirvFrag;
    result = slangProgram->getEntryPointCode(1, 0, spirvFrag.writeRef(), diagnostics.writeRef());
    if (SLANG_FAILED(result))
    {
        spdlog::error("Failed to extract Fragment SPIR-V: {}",
            diagnostics ? static_cast<const char *>(diagnostics->getBufferPointer()) : "Unknown error");
        return ShaderHandle{Handle::Invalid()};
    }

    VkShaderModuleCreateInfo vertInfo {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = spirvVertex->getBufferSize(),
        .pCode = static_cast<const uint32*>(spirvVertex->getBufferPointer())
    };

    VkDevice logicalDevice = device->GetLogicalDevice();

    VkShaderModule vertexModule {};
    if (vkCreateShaderModule(logicalDevice, &vertInfo, nullptr, &vertexModule) != VK_SUCCESS) {
        spdlog::error("Failed to create shader module");
        return ShaderHandle{Handle::Invalid()};
    }

    VkShaderModuleCreateInfo fragInfo{
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = spirvFrag->getBufferSize(),
        .pCode = static_cast<const uint32_t*>(spirvFrag->getBufferPointer())
    };

    VkShaderModule fragmentModule{};
    if (vkCreateShaderModule(logicalDevice, &fragInfo, nullptr, &fragmentModule) != VK_SUCCESS)
    {
        spdlog::error("Vulkan failed to create Fragment Shader Module");
        vkDestroyShaderModule(logicalDevice, vertexModule, nullptr);
        return ShaderHandle{Handle::Invalid()};
    }

    VulkanShader shader = {
        std::move(slangProgram),
        vertexModule,
        fragmentModule
    };
    const uint32 nextIndex = static_cast<uint32>(shaders.size());
    shaders.push_back(shader);
    return ShaderHandle{Handle::Create(nextIndex)};
}

void VulkanRHI::DestroyShader(ShaderHandle handle) {
    VulkanShader& shader = shaders[handle.resourceId.index];
    VkDevice logicalDevice = device->GetLogicalDevice();

    vkDestroyShaderModule(logicalDevice, shader.vertexModule, nullptr);
    vkDestroyShaderModule(logicalDevice, shader.fragmentModule, nullptr);
}

void VulkanRHI::DestroyShader(VulkanShader& shader, VkDevice logicalDevice)
{
    vkDestroyShaderModule(logicalDevice, shader.vertexModule, nullptr);
    vkDestroyShaderModule(logicalDevice, shader.fragmentModule, nullptr);
}

RHIShader * VulkanRHI::GetShader(ShaderHandle ShaderHandle) {
    return &shaders[ShaderHandle.resourceId.index];
}


//-----------------------------------------------------------------------
// BUFFERS
//-----------------------------------------------------------------------
BufferHandle VulkanRHI::CreateBuffer(const BufferDesc & desc) {

    VkBuffer buffer = VK_NULL_HANDLE;
    VmaAllocation allocation = VK_NULL_HANDLE;

    VkBufferCreateInfo bufferCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = desc.size,
        .usage = Transformations::VulkanBufferUsageFlags(desc.usageFlags),
        .sharingMode = desc.sharingMode == SharingMode::EXCLUSIVE ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT
    };

    VmaAllocationCreateInfo vmaAllocationCreateInfo = {
        .flags = Transformations::VulkanMemoryUsageFlags(desc.memoryUsageStrategy),
        .usage = VMA_MEMORY_USAGE_AUTO
    };
    VmaAllocationInfo allocInfo;
    VkResult result = vmaCreateBuffer(device->GetAllocator(), &bufferCreateInfo, &vmaAllocationCreateInfo, &buffer, &allocation, &allocInfo);
    if (result != VK_SUCCESS)
    {
        // buffer failed to init
        spdlog::error("Failed to create Vulkan buffer {}", (int)result);
        return BufferHandle { Handle::Invalid() };
    }

    VulkanBuffer vulkanBuffer {
        allocInfo.pMappedData,
        buffer,
        allocation
    };

    const uint32 nextIndex = static_cast<uint32>(buffers.size());
    buffers.push_back(std::move(vulkanBuffer));

    return BufferHandle { Handle::Create(nextIndex )};
}

void VulkanRHI::HostCopyBuffer(BufferHandle bufferHandle, const void * src, size_t size, size_t offset)
{
    VulkanBuffer & buffer = buffers[bufferHandle.handle.index];
    assert(buffer.mappedData != nullptr && "Fatal: Attempted to MemCopy into an unmapped or GPU_ONLY buffer.");
    memcpy(static_cast<char*>(buffer.mappedData) + offset, src, size);

}

RHIBuffer &VulkanRHI::GetBuffer(BufferHandle bufferHandle)
{
    return buffers[bufferHandle.handle.index];
}


CopyRequest VulkanRHI::RecordCopyBuffer(BufferHandle src, BufferHandle dst, uint64 size, size_t srcOffset, size_t dstOffset)
{
    return {
        .src = src.handle,
        .dst = dst.handle,
        .region = {
            .srcOffset = srcOffset,
            .dstOffset = dstOffset,
            .size = size
        }
    };
}

void VulkanRHI::SubmitCopyBuffer(std::vector<CopyRequest> copyRequests)
{
    //get the queue for tranfer
    VulkanQueue* transferQueue = device->GetTransferQueue();

    //pool is already alive so now we need to create the command buffer
    VulkanCommandPool* transferPool = transferQueue->GetCommandPool(0);

    VulkanCommandBuffer * transferBuffer =
        static_cast<VulkanCommandBuffer*>(
            transferPool->AllocateCommandBuffer(COMMAND_BUFFER_LEVEL_PRIMARY)
            );
    transferBuffer->Begin();

    for (const CopyRequest& request : copyRequests)
    {
        VulkanBuffer& srcBuffer = buffers[request.src.index];
        VulkanBuffer& dstBuffer = buffers[request.dst.index];

        VkBufferCopy copyRegion = {
            .srcOffset = request.region.srcOffset,
            .dstOffset = request.region.dstOffset,
            .size = request.region.size
        };

        vkCmdCopyBuffer(
            transferBuffer->GetVkCommandBuffer(),
            srcBuffer.buffer,
            dstBuffer.buffer,
            1,
            &copyRegion
        );
    }
    transferBuffer->End();

    VkCommandBuffer cmdBuffer = transferBuffer->GetVkCommandBuffer();

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmdBuffer;

   // transferQueue->SubmitCommandBuffer(*frameSync, *transferBuffer);
    vkQueueSubmit(transferQueue->GetVkQueue(), 1, &submitInfo, VK_NULL_HANDLE);

    vkQueueWaitIdle(transferQueue->GetVkQueue());

    vkFreeCommandBuffers(device->GetLogicalDevice(), transferPool->GetVkCommandPool(), 1, &cmdBuffer);

}

// void VulkanBuffer::Map()
// {
//     if (mappedData != nullptr)
//     {
//         spdlog::warn("VulkanBuffer: Buffer is already mapped");
//         return;
//     }
//
//     vmaMapMemory(device->GetAllocator(), allocation, &mappedData);
// }
//
// void VulkanBuffer::Unmap()
// {
//     if (mappedData == nullptr)
//     {
//         spdlog::warn("VulkanBuffer: Buffer is not mapped");
//         return;
//     }
//
//     vmaUnmapMemory(device->GetAllocator(), allocation);
//     mappedData = nullptr;
// }

void VulkanRHI::DestroyBuffer(BufferHandle bufferHandle)
{
    VulkanBuffer& vulkanBuffer = buffers[bufferHandle.handle.index];

    if (vulkanBuffer.buffer != VK_NULL_HANDLE)
    {
        vmaDestroyBuffer(device->GetAllocator(), vulkanBuffer.buffer, vulkanBuffer.allocation);
        vulkanBuffer.buffer = VK_NULL_HANDLE;
        vulkanBuffer.allocation = VK_NULL_HANDLE;
        vulkanBuffer.mappedData = nullptr;
    }
    //should remove it from the slot
    //buffers.destroySlot(bufferHandle.handle);
}

void VulkanRHI::DestroyBuffer(VulkanBuffer & vulkanBuffer, VmaAllocator allocator)
{
    if (vulkanBuffer.buffer != VK_NULL_HANDLE)
    {
        vmaDestroyBuffer(allocator, vulkanBuffer.buffer, vulkanBuffer.allocation);
        vulkanBuffer.buffer = VK_NULL_HANDLE;
        vulkanBuffer.allocation = VK_NULL_HANDLE;
        vulkanBuffer.mappedData = nullptr;
    }
}

/*
* uint32 VulkanRHI::CreatePipeline(const GraphicsPipelineDesc& desc)
{
    size_t hash = hashPipeline(desc);
    //check if pipeline exists
     if (pipelineCache.contains(hash))
     {
         return pipelineCache[hash];
     }
    VulkanGraphicsPipeline pipeline = VulkanGraphicsPipeline(*device);
    pipeline.Initialize(desc);

    pipelineStorage.push_back(pipeline);
    size_t index = pipelineStorage.size();

    pipelineCache[hash] = static_cast<uint32>(index);

    return false;
}
 *
 */

PipelineHandle VulkanRHI::CreatePipeline(const GraphicsPipelineDesc & desc) {

    VulkanShader& shader = shaders[desc.shader.resourceId.index];
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    VkPipeline pipeline = VK_NULL_HANDLE;

    // vertices
    VkVertexInputBindingDescription vertexBinding{
        .binding = 0,
        .stride = sizeof(Vertex),
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
   };

    //TODO: add this on the pipeline desc
    std::vector<VkVertexInputAttributeDescription> vertexAttributes{
            { .location = 0, .binding = 0, .format = VK_FORMAT_R32G32B32_SFLOAT },
            { .location = 1, .binding = 0, .format = VK_FORMAT_R32G32B32_SFLOAT, .offset = offsetof(Vertex, normal) },
            { .location = 2, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = offsetof(Vertex, uvs) },
            { .location = 3, .binding = 0, .format = VK_FORMAT_R32G32B32_SFLOAT, .offset = offsetof(Vertex, color) },
        };

    VkPipelineVertexInputStateCreateInfo vertexInputState{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount = 1,
        .pVertexBindingDescriptions = &vertexBinding,
        .vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexAttributes.size()),
        .pVertexAttributeDescriptions = vertexAttributes.data()
    };

    //topology
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
    };

    //shaders
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages{
            { .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
              .stage = VK_SHADER_STAGE_VERTEX_BIT,
              .module = shader.vertexModule, .pName = "main"},
            { .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
              .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
              .module = shader.fragmentModule, .pName = "main" }
    };

    //viewport
    VkPipelineViewportStateCreateInfo viewportState{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = desc.viewportCount,
        .scissorCount = desc.scissorCount,
    };
    // care as this might make the viewport immutable. Will need to check

    std::vector<VkDynamicState> dynamicStates{
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = 2,
        .pDynamicStates = dynamicStates.data()
    };

    //depth buffering
    VkPipelineDepthStencilStateCreateInfo depthStencilState{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .depthTestEnable = desc.depthTestEnable,
        .depthWriteEnable = desc.depthWriteEnable,
        .depthCompareOp = VulkanCompareOps[desc.depthCompareOp]
    };

    size_t imageFormatsSize = desc.imageFormats.size() > 0 ? desc.imageFormats.size() : 0;
    std::vector<VkFormat> imageFormats;
    for (size_t i = 0; i < imageFormatsSize; ++i)
    {
        imageFormats.push_back(VulkanFormats[desc.imageFormats[i]]);
    }

    //dynamic rendering
    VkPipelineRenderingCreateInfo renderingCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
        .colorAttachmentCount = static_cast<uint32>(imageFormatsSize),
        .pColorAttachmentFormats = &imageFormats.front(),
        .depthAttachmentFormat = VulkanFormats[desc.depthImageFormat]
    };

    VkPipelineRasterizationStateCreateInfo rasterizationState{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .polygonMode = VulkanPolygonModes[desc.polygonMode],
        .cullMode = VulkanCullModes[desc.cullMode],
        .frontFace = VulkanFrontFace[desc.frontFace],
        .depthBiasEnable = VK_FALSE,
        .lineWidth = 1.0f
    };


    VkPipelineMultisampleStateCreateInfo multisampleState{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable = VK_FALSE
    };

    //colour blending. Still don't understand it 100%. Disabled
    VkPipelineColorBlendAttachmentState blendAttachment{
        .blendEnable = VK_FALSE,
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    };

    VkPipelineColorBlendStateCreateInfo colorBlendState{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .logicOpEnable = VK_FALSE,
        .attachmentCount = 1,
        .pAttachments = &blendAttachment
    };

    //pipeline layout
    VkPushConstantRange pushConstantRange{
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
        .size = sizeof(VkDeviceAddress)
    };
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 0,
        .pSetLayouts = nullptr,
        .pushConstantRangeCount = 1,
        .pPushConstantRanges = &pushConstantRange
    };

    VkDevice logicalDevice = device->GetLogicalDevice();
    vkCreatePipelineLayout(logicalDevice, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout);

    VkGraphicsPipelineCreateInfo pipelineCreateInfo{
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext = &renderingCreateInfo,
        .stageCount = 2,
        .pStages = shaderStages.data(),
        .pVertexInputState = &vertexInputState,
        .pInputAssemblyState = &inputAssemblyState,
        .pViewportState = &viewportState,
        .pRasterizationState = &rasterizationState,
        .pMultisampleState = &multisampleState,
        .pDepthStencilState = &depthStencilState,
        .pColorBlendState = &colorBlendState,
        .pDynamicState = &dynamicState,
        .layout = pipelineLayout,
        .renderPass = nullptr
    };

    //for pipeline derivation
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineCreateInfo.basePipelineIndex = -1;


    if (vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &pipeline) != VK_SUCCESS)
    {
        spdlog::error("VulkanRHI: Failed to create graphics pipeline!");
        return PipelineHandle{ Handle::Invalid() };
    }

    //DestroyShader(shader, logicalDevice);
    // vkDestroyShaderModule(device->GetLogicalDevice(), vertexModule, nullptr);
    //vkDestroyShaderModule(device->GetLogicalDevice(), fragModule, nullptr);

    VulkanGraphicsPipeline vulkanPipeline{
        pipeline,
        pipelineLayout
    };
    uint32 nextIndex = static_cast<uint32>(pipelines.size());
    pipelines.push_back(vulkanPipeline);
    return PipelineHandle{ Handle::Create(nextIndex) };
}

void VulkanRHI::DestroyPipeline(PipelineHandle pipelineHandle) {
}

void VulkanRHI::DestroyPipeline(VulkanGraphicsPipeline& vulkanPipeline, VkDevice logicalDevice) {

    if (vulkanPipeline.pipeline != VK_NULL_HANDLE)
    {
        vkDestroyPipeline(logicalDevice, vulkanPipeline.pipeline, nullptr);
        vulkanPipeline.pipeline = VK_NULL_HANDLE;
    }

    if (vulkanPipeline.pipelineLayout != VK_NULL_HANDLE)
    {
        vkDestroyPipelineLayout(logicalDevice, vulkanPipeline.pipelineLayout, nullptr);
        vulkanPipeline.pipelineLayout = VK_NULL_HANDLE;
    }
}


// }
    //
    // mesh.buffer->CopyData(mesh.vertices.data(), mesh.vertices.size() * sizeof(Vertex));
    // mesh.buffer->CopyData(mesh.indices.data(), mesh.indices.size() * sizeof(uint32), mesh.vertices.size() * sizeof(Vertex));
    //
    // VulkanBuffer & vBuffer = static_cast<VulkanBuffer &>(*mesh.buffer);
    //        memcpy(static_cast<char*>(dest) + offset, src, size);

    //
    // VkDeviceSize vBufSize{ sizeof(Vertex) * 3 };
    // VkDeviceSize vOffset{ 0 };
    // vkCmdBindVertexBuffers(cmd->GetVkCommandBuffer(), 0, 1, &vBuffer.buffer, &vOffset);
    // vkCmdBindIndexBuffer(cmd->GetVkCommandBuffer(), vBuffer.buffer, vBufSize, VK_INDEX_TYPE_UINT16);

bool VulkanRHI::BeginFrame()
{
    Validate(); // hard assertion

    //TODO: work this out.
    // i dont like this as i introduce an extra variable
    // when i already have frameSync->currentFrame and I should be using this,
    // although current frame shouldnt change on a resize.
    uint32 currentFrame = frameSync->BeginSynchronize();

    if (currentFrame == 666)
    {
        return false;
    }

    cmd = &device->GetGraphicsQueue()->GetCommandPool(currentFrame)->GetCommandBuffer();

    cmd->Reset();
    cmd->Begin();

    return true;
}

void VulkanRHI::EndFrame()
{
    //end command buffer
    cmd->End();
    //synchronize with the semaphores during submission
    device->GetGraphicsQueue()->SubmitCommandBuffer(*frameSync, *cmd);
    swapchain->Present(*frameSync);

    frameSync->EndSynchronize();
}

void VulkanRHI::ClearColour(Float3 colour)
{
    VkClearColorValue clearColor = {};
    clearColor.float32[0] = colour.R;
    clearColor.float32[1] = colour.G;
    clearColor.float32[2] = colour.B;
    clearColor.float32[3] = 1.0f;

    VkImageSubresourceRange clearRange = VkImageSubresourceRange {
        VK_IMAGE_ASPECT_COLOR_BIT,
        0,
        1,
        0,
        1
    };

    //clear image
    vkCmdClearColorImage(cmd->GetVkCommandBuffer(), swapchain->GetVkImage(frameSync->imageIndex), VK_IMAGE_LAYOUT_GENERAL, &clearColor, 1, &clearRange);

}


void VulkanRHI::BeginRendering()
{
    VkClearColorValue clearColor = {};
    clearColor.float32[0] = 0.0f;
    clearColor.float32[1] = 0.0f;
    clearColor.float32[2] = 0.0f;
    clearColor.float32[3] = 1.0f;

    VkRenderingAttachmentInfo attachmentInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
        .imageView = swapchain->GetVkImageView(frameSync->imageIndex),
        .imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .clearValue = clearColor
    };

    //setup rendering info (pass this)
    VkRenderingInfo renderingInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
        .renderArea = { .offset = { 0, 0 }, .extent = swapchain->GetVkExtent() },
        .layerCount = 1,
        .colorAttachmentCount = 1,
        .pColorAttachments = &attachmentInfo
    };

    vkCmdBeginRendering(cmd->GetVkCommandBuffer(), &renderingInfo);

}

void VulkanRHI::EndRendering()
{
    vkCmdEndRendering(cmd->GetVkCommandBuffer());
}

void VulkanRHI::BindPipeline(uint32_t pipelineID)
{
        // if (pipelineID >= pipelineStorage.size())
        // {
        //     spdlog::error("VulkanRHI: Invalid pipeline ID {}", pipelineID);
        //     return;
        // }
        //
        // VulkanGraphicsPipeline pipeline = pipelineStorage[pipelineID];
        // vkCmdBindPipeline(cmd->GetVkCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);
}

void VulkanRHI::PrepareVertexBuffer(Handle meshHandle)
{

    // VkDeviceSize vBufSize{ sizeof(Vertex) * 3 };
    // VkDeviceSize vOffset{ 0 };
    // vkCmdBindVertexBuffers(cmd->GetVkCommandBuffer(), 0, 1, &vBuffer.buffer, &vOffset);
    // vkCmdBindIndexBuffer(cmd->GetVkCommandBuffer(), vBuffer.buffer, vBufSize, VK_INDEX_TYPE_UINT32);
}

void VulkanRHI::PrepareVertexBuffer(uint32_t bufferID)
{
}

void VulkanRHI::BindIndexBuffer(uint32_t bufferID)
{
}

void VulkanRHI::Draw()
{
    VkViewport viewport = {
        0.0f,
        0.0f,
        static_cast<float>(swapchain->GetVkExtent().width),
        static_cast<float>(swapchain->GetVkExtent().height),
        0.0f,
        1.0f
    };

    vkCmdSetViewport(
        cmd->GetVkCommandBuffer(),
        0,
        1,
        &viewport);

    VkRect2D scissors = VkRect2D(VkOffset2D(0, 0),
        swapchain->GetVkExtent());

    vkCmdSetScissor(
        cmd->GetVkCommandBuffer(),
        0,
        1,
        &scissors
    );

    vkCmdDraw(cmd->GetVkCommandBuffer(),3,1,0,0);
}

void VulkanRHI::WaitIdle()
{
    device->WaitIdle();
}

void VulkanRHI::TransitionBarrier(const ImageBarrier& barrier)
{
    VkImageMemoryBarrier2 imageBarrier {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
        .pNext = nullptr,
        .srcStageMask = VulkanStageFlags(barrier.sourceStageMask),
        .srcAccessMask = VulkanAccessFlags(barrier.sourceAccessMask),
        .dstStageMask = VulkanStageFlags(barrier.destStageMask),
        .dstAccessMask = VulkanAccessFlags(barrier.destAccessMask),
        .oldLayout = VulkanTransitionFlags[barrier.sourceState],
        .newLayout = VulkanTransitionFlags[barrier.destState],
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = swapchain->GetVkImage(frameSync->imageIndex),
        .subresourceRange = {
            .aspectMask = VulkanImageAspectFlags(barrier.aspectMask),
            .baseMipLevel = barrier.baseMipLevel,
            .levelCount = barrier.levelCount,
            .baseArrayLayer = barrier.baseArrayLayer,
            .layerCount = barrier.layerCount
        }
    };

    VkDependencyInfo dependencyInfo {
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .pNext = nullptr,
        .imageMemoryBarrierCount = 1,
        .pImageMemoryBarriers = &imageBarrier
    };

    vkCmdPipelineBarrier2(cmd->GetVkCommandBuffer(), &dependencyInfo);
}


std::vector<const char*> VulkanRHI::GetVulkanRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    auto vulkanExtensions = std::vector<const char*>(glfwExtensions, glfwExtensions + glfwExtensionCount);

    // if (enableValidationLayers) {
    //     deviceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    // }

    return vulkanExtensions;
}
