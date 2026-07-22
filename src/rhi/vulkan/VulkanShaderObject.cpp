//
// Created by nutellis on 16/07/2026.
//

#include "rhi/vulkan/VulkanShaderObject.h"

#include <volk.h>
#include <slang.h>


namespace {
    // VkDescriptorType mapToDescriptorType(slang::BindingType bindingRangeType)
    // {
    //     switch(bindingRangeType)
    //     {
    //         case slang::BindingType::Texture:
    //             return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    //         case slang::BindingType::MutableTexture:
    //             return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    //             ...
    //             }
    // }
}
VulkanShaderObject::~VulkanShaderObject() {
}

void VulkanShaderObject::write(ShaderOffset offset, const void *data, size_t size) {
}


// void VulkanShaderObject::write(ShaderOffset offset, Texture* texture)
// {
//     uint32_t bindingIndex = m_typeLayout->getBindingRangeFirstDescriptorRangeIndex(
//         offset.bindingRangeIndex);
//
//     VkDescriptorImageInfo image;
//     image.imageView = texture->getVulkanImageView();
//     image.imageLayout = texture->getVulkanImageLayout();
//
//     VkWriteDescriptorSet write = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, 0};
//     write.dstSet = m_descriptorSet;
//     write.dstBinding = bindingIndex;
//     write.dstArrayElement = offset.arrayIndexInBindingRange;
//     write.descriptorCount = 1;
//     write.descriptorType = mapToDescriptorType(m_typeLayout->getBindingRangeType());
//     write.pImageInfo = &image;
//
//     vkUpdateDescriptorSets(vulkanDevice,
//         1, &write,
//         0, nullptr);
// }