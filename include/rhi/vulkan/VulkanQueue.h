//
// Created by Nutellis on 13-Feb-26.
//

#ifndef ALKYONERENDERENGINE_VULKANCOMMANDQUEUE_H
#define ALKYONERENDERENGINE_VULKANCOMMANDQUEUE_H

#include <array>

#include "rhi/core/ICommandQueue.h"

#include <volk.h>

#include <core/PODTypes.h>

#include "rhi/core/IFrameSync.h"

class VulkanCommandPool;
class VulkanDevice;


class VulkanQueue: public ICommandQueue
{
public:
    VulkanQueue() = delete;
    VulkanQueue(VulkanDevice& device, uint32 inFamilyIndex, CommandQueueType inType);
    ~VulkanQueue() override;


    bool Initialize() override;
    void Terminate() override;

    VkQueue GetVkQueue();
    VkQueueFlags GetVkQueueFlags() const;
    uint32 GetFamilyIndex() const;

    VulkanCommandPool* GetCommandPool(uint32 frameIndex);

    void AllocateCommandBuffers(uint32 count) override;
    void SubmitCommandBuffer(IFrameSync& sync, ICommandBuffer& cmdBuffer) override;

private:

    VulkanDevice & parentDevice;
    VkQueue queue = VK_NULL_HANDLE;
    uint32 familyIndex {};
    uint32 queueIndex {}; // by default 0

    VkQueueFlags queueType{};

    std::array<VulkanCommandPool*, FRAMES_IN_FLIGHT> pools;

};


#endif //ALKYONERENDERENGINE_VULKANCOMMANDQUEUE_H