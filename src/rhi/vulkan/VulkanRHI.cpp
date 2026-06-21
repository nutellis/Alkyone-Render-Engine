//
// Created by Nutellis on 20-Jan-26.
//

#include <rhi/vulkan/VulkanRHI.h>

#define VOLK_IMPLEMENTATION

#include <core/ApplicationWindow.h>

#include <rhi/core/IGraphicsPipeline.h>
#include "rhi/vulkan/VulkanCommandPool.h"
#include "rhi/vulkan/VulkanFrameSync.h"

#include <set>

#include "rendering/Mesh.h"
#include "rendering/types/Vertex.h"
#include "rhi/vulkan/VulkanBuffer.h"
#include "rhi/vulkan/VulkanCommandBuffer.h"
#include "rhi/vulkan/VulkanDefinitions.h"
#include "rhi/vulkan/VulkanGraphicsPipeline.h"
#include "rhi/vulkan/VulkanQueue.h"
#include "spdlog/spdlog.h"

#include <utilities/TypeUtilities.h>

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
    }

    if (CreateInstance() == false)
    {
        spdlog::error("Alkyone RHI: Critical failure, failed to initialize Vulkan Instance");
        return false;
    }

    device = new VulkanDevice(instance);
    swapchain = new VulkanSwapchain(*device, *window);
    frameSync = new VulkanFrameSync(*device, *swapchain);

    if (device->Initialize()
        && swapchain->Initialize()
        && frameSync->Initialize()
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

    spdlog::info("Alkyone RHI: Context Initialized Successfully");
    return true;
}

void VulkanRHI::Terminate()
{
    for (auto pipeline : pipelineStorage)
    {
        pipeline->DestroyPipeline();
        delete pipeline;
    }

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

uint32 VulkanRHI::CreatePipeline(const GraphicsPipelineDesc& desc)
{
    size_t hash = hashPipeline(desc);
    //check if pipeline exists
     if (pipelineCache.contains(hash))
     {
         return pipelineCache[hash];
     }
    VulkanGraphicsPipeline * pipeline = new VulkanGraphicsPipeline(*device);
    pipeline->CreatePipeline(desc);

    pipelineStorage.push_back(std::move(pipeline));
    size_t index = pipelineStorage.size();

    pipelineCache[hash] = static_cast<uint32>(index);

    return false;
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

    cmd = device->GetGraphicsQueue()->GetCommandPool(currentFrame)->GetCommandBuffer();

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
        if (pipelineID >= pipelineStorage.size())
        {
            spdlog::error("VulkanRHI: Invalid pipeline ID {}", pipelineID);
            return;
        }

        VulkanGraphicsPipeline* pipeline = pipelineStorage[pipelineID];
        vkCmdBindPipeline(cmd->GetVkCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetVkPipeline());
}

void VulkanRHI::PrepareVertexBuffer(Mesh & mesh)
{
    // if (mesh.buffer == nullptr)
    // {
    //     mesh.buffer = new VulkanBuffer(*device);
    //     BufferDesc desc = {
    //         .size = mesh.vertices.size() * sizeof(Vertex) + mesh.indices.size() * sizeof(uint32),
    //         .usageFlags = BufferTypeBits::VERTEX_BUFFER | BufferTypeBits::INDEX_BUFFER,
    //         .sharingMode = SharingMode::EXCLUSIVE
    //     };
    //     mesh.buffer->Initialize(desc);
    // }
    //
    // mesh.buffer->CopyData(mesh.vertices.data(), mesh.vertices.size() * sizeof(Vertex));
    // mesh.buffer->CopyData(mesh.indices.data(), mesh.indices.size() * sizeof(uint32), mesh.vertices.size() * sizeof(Vertex));
    //
    // VulkanBuffer & vBuffer = static_cast<VulkanBuffer &>(*mesh.buffer);
    //
    // VkDeviceSize vBufSize{ sizeof(Vertex) * 3 };
    // VkDeviceSize vOffset{ 0 };
    // vkCmdBindVertexBuffers(cmd->GetVkCommandBuffer(), 0, 1, &vBuffer.buffer, &vOffset);
    // vkCmdBindIndexBuffer(cmd->GetVkCommandBuffer(), vBuffer.buffer, vBufSize, VK_INDEX_TYPE_UINT16);
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
