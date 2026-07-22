//
// Created by Nutellis on 18-Feb-26.
//

#ifndef ALKYONERENDERENGINE_VULKANCOMMANDBUFFER_H
#define ALKYONERENDERENGINE_VULKANCOMMANDBUFFER_H
#include "rhi/core/ICommandBuffer.h"
#include "volk.h"

class VulkanDevice;

class VulkanCommandBuffer: public ICommandBuffer
{

public:
    VulkanCommandBuffer(VulkanDevice& device, VkCommandBuffer buffer, CommandBufferLevel bufferLevel);
    ~VulkanCommandBuffer() override;
    bool Initialize() override;
    void Terminate() override;

    [[nodiscard]] VkCommandBuffer GetVkCommandBuffer() const;
    void Begin() override;
    void Reset() override;
    void End();

    void Barrier(ImageBarrier barrier) override;

private:
    VkCommandBufferLevel bufferLevel {};
    VkCommandBuffer commandBuffer = VK_NULL_HANDLE;

    // Public immutable reference to be accessed directly
    VulkanDevice& device;
};


#endif //ALKYONERENDERENGINE_VULKANCOMMANDBUFFER_H