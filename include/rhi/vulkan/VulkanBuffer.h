//
// Created by Nutellis on 08-Mar-26.
//

#ifndef ALKYONERENDERENGINE_VULKANBUFFER_H
#define ALKYONERENDERENGINE_VULKANBUFFER_H

#include "rhi/core/IBuffer.h"
#include <volk.h>

#include "vk_mem_alloc.h"

class VulkanDevice;


class VulkanBuffer : public IBuffer
{
public:
    VulkanBuffer() = delete;
    VulkanBuffer(VulkanDevice& device);
    ~VulkanBuffer() override;

    bool Initialize(BufferDesc desc) override;

    void Terminate() override;


    void CopyData(const void* data, uint64 size, uint64 offset) override;

    VulkanDevice & device;

    VkBuffer buffer = VK_NULL_HANDLE;
    VmaAllocation allocation = VK_NULL_HANDLE;
};


#endif //ALKYONERENDERENGINE_VULKANBUFFER_H