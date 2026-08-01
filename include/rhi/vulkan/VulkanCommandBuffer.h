//
// Created by Nutellis on 18-Feb-26.
//

#ifndef ALKYONERENDERENGINE_VULKANCOMMANDBUFFER_H
#define ALKYONERENDERENGINE_VULKANCOMMANDBUFFER_H
#include "rhi/core/RHICommandBuffer.h"
#include "volk.h"

class VulkanDevice;

struct VulkanCommandBuffer: public RHICommandBuffer
{

public:

    VulkanCommandBuffer();
    VulkanCommandBuffer(VkCommandBuffer buffer, CommandBufferLevel bufferLevel);

    // ~VulkanCommandBuffer() override;
    // bool Initialize() override;
    // void Terminate() override;
    //
    // [[nodiscard]] VkCommandBuffer GetVkCommandBuffer() const;
    // void Begin() override;
    // void Reset() override;
    // void End();
    //
    // void Barrier(ImageBarrier barrier) override;

    VkCommandBufferLevel bufferLevel {};
    VkCommandBuffer commandBuffer = VK_NULL_HANDLE;

};


#endif //ALKYONERENDERENGINE_VULKANCOMMANDBUFFER_H