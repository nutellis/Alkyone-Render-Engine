//
// Created by Nutellis on 11-Feb-26.
//

#ifndef ALKYONERENDERENGINE_VULKANGRAPHICSCOMMANDPOOL_H
#define ALKYONERENDERENGINE_VULKANGRAPHICSCOMMANDPOOL_H

#include <vector>

#include <rhi/core/ICommandPool.h>
#include "VulkanDevice.h"

class VulkanCommandBuffer;
struct CommandPoolDesc;


class VulkanCommandPool: public ICommandPool
{
public:
    VulkanCommandPool() = delete;
    VulkanCommandPool(VulkanDevice& device, uint32 queueFamilyIndex);
    ~VulkanCommandPool() override;


    bool Initialize() override;
    void Terminate() override;

    ICommandBuffer* AllocateCommandBuffer(CommandBufferLevel bufferLevel) override;

    [[nodiscard]] VulkanCommandBuffer * GetCommandBuffer() const;

private:
    VulkanDevice & device;

    uint32 queueFamilyIndex;

    VkCommandPool commandPool = VK_NULL_HANDLE;

    //std::vector<VulkanCommandBuffer *> commandBuffers;
    //TODO: this is a temp solution
    VulkanCommandBuffer * commandBuffer;

};


#endif //ALKYONERENDERENGINE_VULKANGRAPHICSCOMMANDPOOL_H