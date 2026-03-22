//
// Created by Nutellis on 20-Feb-26.
//

#include "rhi/vulkan/VulkanImage.h"

VulkanImage::~VulkanImage()
{
}

VkImage VulkanImage::GetImage() const
{
    return image;
}

TextureUsage VulkanImage::GetTextureUsage() const
{
    return usage;
}
