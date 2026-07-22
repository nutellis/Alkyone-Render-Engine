//
// Created by Nutellis on 30-Jan-26.
//

#ifndef ALKYONERENDERENGINE_VULKANGRAPHICSPIPELINE_H
#define ALKYONERENDERENGINE_VULKANGRAPHICSPIPELINE_H
#include <volk.h>

#include <rhi/core/RHIGraphicsPipeline.h>

struct VulkanGraphicsPipeline: RHIGraphicsPipeline
{
    VulkanGraphicsPipeline(VkPipeline pipeline, VkPipelineLayout pipeline_layout) :
    pipelineLayout(pipeline_layout), pipeline(pipeline) {}

    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    VkPipeline pipeline = VK_NULL_HANDLE;

};


#endif //ALKYONERENDERENGINE_VULKANGRAPHICSPIPELINE_H