//
// Created by Nutellis on 30-Jan-26.
//

#ifndef ALKYONERENDERENGINE_VULKANGRAPHICSPIPELINE_H
#define ALKYONERENDERENGINE_VULKANGRAPHICSPIPELINE_H
#include <volk.h>

#include <rhi/core/IGraphicsPipeline.h>


class VulkanDevice;

class VulkanGraphicsPipeline: IGraphicsPipeline
{
public:
    VulkanGraphicsPipeline(VulkanDevice& device);
    ~VulkanGraphicsPipeline() override;

    bool CreatePipeline(GraphicsPipelineDesc desc) override;
    void DestroyPipeline() override;
    void BindPipeline() ;

    VkPipeline GetVkPipeline() const;


private:
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    VkPipeline pipeline = VK_NULL_HANDLE;

    VulkanDevice & device;
};


#endif //ALKYONERENDERENGINE_VULKANGRAPHICSPIPELINE_H