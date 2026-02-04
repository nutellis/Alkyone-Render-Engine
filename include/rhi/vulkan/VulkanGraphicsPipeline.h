//
// Created by Nutellis on 30-Jan-26.
//

#ifndef ALKYONERENDERENGINE_VULKANGRAPHICSPIPELINE_H
#define ALKYONERENDERENGINE_VULKANGRAPHICSPIPELINE_H
#include <vulkan/vulkan_core.h>

#include "rhi/IGraphicsPipeline.h"


class VulkanGraphicsPipeline
{
public:
    static VkPipeline CreatePipeline(GraphicsPipelineDesc desc) ;
    void DestroyPipeline() ;
    void BindPipeline() ;


private:
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
};


#endif //ALKYONERENDERENGINE_VULKANGRAPHICSPIPELINE_H