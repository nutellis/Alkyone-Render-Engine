//
// Created by Nutellis on 11-Feb-26.
//

#ifndef ALKYONERENDERENGINE_VULKANGRAPHICSCOMMANDPOOL_H
#define ALKYONERENDERENGINE_VULKANGRAPHICSCOMMANDPOOL_H

#include <vector>

#include <rhi/core/RHICommandPool.h>
#include "VulkanDevice.h"

class VulkanCommandBuffer;
struct CommandPoolDesc;


struct  VulkanCommandPool: public RHICommandPool
{
public:
    VulkanCommandPool();
    VulkanCommandPool(VkCommandPool pool) : commandPool(pool) {}

    // ~VulkanCommandPool() override;
    //
    //
    // bool Initialize() override;
    // void Terminate() override;
    //
    // ICommandBuffer* AllocateCommandBuffer(CommandBufferLevel bufferLevel) override;
    //
    // [[nodiscard]] VulkanCommandBuffer& GetCommandBuffer() const;
    //
    // [[nodiscard]] VkCommandPool GetVkCommandPool() const;

    VkCommandPool commandPool = VK_NULL_HANDLE;
};


#endif //ALKYONERENDERENGINE_VULKANGRAPHICSCOMMANDPOOL_H