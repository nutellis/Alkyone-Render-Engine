//
// Created by nutellis on 17/07/2026.
//

#ifndef ALKYONERENDERENGINE_VULKANSHADER_H
#define ALKYONERENDERENGINE_VULKANSHADER_H

#include "rhi/core/shader/RHIShader.h"
#include <volk.h>


struct VulkanShader : public RHIShader
{
    VulkanShader(
        Slang::ComPtr<slang::IComponentType> program,
        VkShaderModule vertexModule,
        VkShaderModule fragmentModule)
        : RHIShader(std::move(program))
        , vertexModule(vertexModule)
        , fragmentModule(fragmentModule)
    {
    }

    VkShaderModule vertexModule = VK_NULL_HANDLE;
    VkShaderModule fragmentModule = VK_NULL_HANDLE;
};


#endif //ALKYONERENDERENGINE_VULKANSHADER_H