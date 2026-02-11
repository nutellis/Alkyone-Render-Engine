//
// Created by Nutellis on 30-Jan-26.
//

#include <rhi/vulkan/VulkanGraphicsPipeline.h>

#include "rendering/types/Vertex.h"

#include "rhi/GraphicsPipelineDesc.h"

#include <rhi/vulkan/VulkanDefinitions.h>

#include "volk.h"
#include "spdlog/spdlog.h"

VkPipeline VulkanGraphicsPipeline::CreatePipeline(GraphicsPipelineDesc desc, VkDevice device)
{
    // vertices
    VkVertexInputBindingDescription vertexBinding{
        .binding = 0,
        .stride = sizeof(Vertex),
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
   };

    std::vector<VkVertexInputAttributeDescription> vertexAttributes{
        { .location = 0, .binding = 0, .format = VK_FORMAT_R32G32B32_SFLOAT },
        { .location = 1, .binding = 0, .format = VK_FORMAT_R32G32B32_SFLOAT, .offset = offsetof(Vertex, normal) },
        { .location = 2, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = offsetof(Vertex, uvs) },
    };

    VkPipelineVertexInputStateCreateInfo vertexInputState{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount = 1,
        .pVertexBindingDescriptions = &vertexBinding,
        .vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexAttributes.size()),
        .pVertexAttributeDescriptions = vertexAttributes.data(),
    };

    //topology
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
    };

    //TODO: MOVE THIS FROM HERE
    Slang::ComPtr<ISlangBlob> spirvVertex;
   // desc.vertexShaderModule->getTargetCode(0, spirvVertex.writeRef());

    SlangResult result = desc.vertexShaderModule->getTargetCode(0, spirvVertex.writeRef());
    if (SLANG_FAILED(result) || !spirvVertex)
    {
        spdlog::error("Failed to get SPIR-V code from vertex shader!");
        return VK_NULL_HANDLE;
    }

    VkShaderModuleCreateInfo vertexModuleCreateInfo{
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = spirvVertex->getBufferSize(),
        .pCode = static_cast<const uint32_t*>(spirvVertex->getBufferPointer())
    };

    VkShaderModule vertexModule{};
    //TODO: add checks
    vkCreateShaderModule(device, &vertexModuleCreateInfo, nullptr, &vertexModule);
    //TODO: MOVE THIS FROM HERE
    Slang::ComPtr<ISlangBlob> spirvFrag;
    desc.fragmentShaderModule->getTargetCode(0, spirvFrag.writeRef());
    VkShaderModuleCreateInfo fragModuleCreateInfo{
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = spirvFrag->getBufferSize(),
        .pCode = static_cast<const uint32_t*>(spirvFrag->getBufferPointer())
    };
    VkShaderModule fragModule{};
    vkCreateShaderModule(device, &fragModuleCreateInfo, nullptr, &fragModule);

    //shaders
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages{
        { .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
          .stage = VK_SHADER_STAGE_VERTEX_BIT,
          .module = vertexModule, .pName = "vertex"},
        { .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
          .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
          .module = fragModule, .pName = "fragment" }
    };


    //viewport
    VkPipelineViewportStateCreateInfo viewportState{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = desc.viewportCount,
        .scissorCount = desc.scissorCount,
    };
    // care as this might make the viewport immutable. Will need to check

    std::vector<VkDynamicState> dynamicStates{
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = 2,
        .pDynamicStates = dynamicStates.data()
    };

    //depth buffering
    VkPipelineDepthStencilStateCreateInfo depthStencilState{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .depthTestEnable = desc.depthTestEnable,
        .depthWriteEnable = desc.depthWriteEnable,
        .depthCompareOp = VulkanCompareOps[desc.depthCompareOp]
    };

    size_t imageFormatsSize = desc.imageFormats.size() > 0 ? desc.imageFormats.size() : 0;
    std::vector<VkFormat> imageFormats;
    for (size_t i = 0; i < imageFormatsSize; ++i)
    {
        imageFormats.push_back(VulkanFormats[desc.imageFormats[i]]);
    }

    //dynamic rendering
    VkPipelineRenderingCreateInfo renderingCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
        .colorAttachmentCount = static_cast<uint32>(imageFormatsSize),
        .pColorAttachmentFormats = &imageFormats.front(),
        .depthAttachmentFormat = VulkanFormats[desc.depthImageFormat]
    };

    VkPipelineRasterizationStateCreateInfo rasterizationState{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .polygonMode = VulkanPolygonModes[desc.polygonMode],
        .cullMode = VulkanCullModes[desc.cullMode],
        .frontFace = VulkanFrontFace[desc.frontFace],
        .depthBiasEnable = VK_FALSE,
        .lineWidth = 1.0f
    };


    VkPipelineMultisampleStateCreateInfo multisampleState{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable = VK_FALSE
    };

    //colour blending. Still don't understand it 100%. Disabled
    VkPipelineColorBlendAttachmentState blendAttachment{
        .blendEnable = VK_FALSE,
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    };

    VkPipelineColorBlendStateCreateInfo colorBlendState{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .logicOpEnable = VK_FALSE,
        .attachmentCount = 1,
        .pAttachments = &blendAttachment
    };

    //pipeline layout
    VkPushConstantRange pushConstantRange{
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
        .size = sizeof(VkDeviceAddress)
    };
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 0,
        .pSetLayouts = nullptr,
        .pushConstantRangeCount = 1,
        .pPushConstantRanges = &pushConstantRange
    };

    vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout);


    VkGraphicsPipelineCreateInfo pipelineCreateInfo{
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext = &renderingCreateInfo,
        .stageCount = 2,
        .pStages = shaderStages.data(),
        .pVertexInputState = &vertexInputState,
        .pInputAssemblyState = &inputAssemblyState,
        .pViewportState = &viewportState,
        .pRasterizationState = &rasterizationState,
        .pMultisampleState = &multisampleState,
        .pDepthStencilState = &depthStencilState,
        .pColorBlendState = &colorBlendState,
        .pDynamicState = &dynamicState,
        .layout = pipelineLayout,
        .renderPass = nullptr
    };

    //for pipeline derivation
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineCreateInfo.basePipelineIndex = -1;


    if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &pipeline) != VK_SUCCESS)
    {
        // pipeline failed to init
        return nullptr;
    }

    return pipeline;

}
