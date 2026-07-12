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
    VulkanBuffer(VulkanDevice* device);
    ~VulkanBuffer() override;

    // no copy!
    VulkanBuffer(const VulkanBuffer& other) = delete;
    VulkanBuffer operator=(const VulkanBuffer& other) = delete;

    //yes move!!
    VulkanBuffer(VulkanBuffer&& other) noexcept;
    VulkanBuffer& operator=(VulkanBuffer&& other) noexcept;

    bool Initialize(BufferDesc desc) override;

    void Terminate() override;
    void Map() override;
    void Unmap() override;

    VkBuffer GetVkBuffer() const;

private:
    VulkanDevice * device;

    void * mappedData = nullptr;

    VkBuffer buffer = VK_NULL_HANDLE;
    VmaAllocation allocation = VK_NULL_HANDLE;
};


#endif //ALKYONERENDERENGINE_VULKANBUFFER_H