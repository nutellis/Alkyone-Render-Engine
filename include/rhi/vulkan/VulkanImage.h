//
// Created by Nutellis on 20-Feb-26.
//

#ifndef ALKYONERENDERENGINE_VULKANIMAGE_H
#define ALKYONERENDERENGINE_VULKANIMAGE_H
#include "rhi/core/ITexture.h"
#include <volk.h>

class VulkanImage: public ITexture
{
public:
    ~VulkanImage() override;

    [[nodiscard]] VkImage GetImage() const;
    [[nodiscard]] TextureUsage GetTextureUsage() const;

private:
    VkImage image = VK_NULL_HANDLE;
    TextureUsage usage = NONE;
};


#endif //ALKYONERENDERENGINE_VULKANIMAGE_H