//
// Created by Nutellis on 08-Mar-26.
//

#ifndef ALKYONERENDERENGINE_VULKANBUFFER_H
#define ALKYONERENDERENGINE_VULKANBUFFER_H

#include "rhi/core/RHIBuffer.h"
#include <volk.h>
#include "vk_mem_alloc.h"

class VulkanDevice;


struct VulkanBuffer : RHIBuffer
{
    VulkanBuffer(void *mappedData, VkBuffer buffer, VmaAllocation allocation) :
        mappedData(mappedData),
        buffer(buffer),
        allocation(allocation)
    {}


    void * mappedData = nullptr;
    VkBuffer buffer = VK_NULL_HANDLE;
    VmaAllocation allocation = VK_NULL_HANDLE;
};


#endif //ALKYONERENDERENGINE_VULKANBUFFER_H