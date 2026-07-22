//
// Created by Nutellis on 30-Jan-26.
//

#ifndef ALKYONERENDERENGINE_VULKANDEFINITIONS_H
#define ALKYONERENDERENGINE_VULKANDEFINITIONS_H

#include <rhi/core/RHIDefinitions.h>
#include <utilities/TypeUtilities.h>

// TODO: REMOVE THIS FILE AND SPREAD EACH FUNC/ARRAY TO THEIR RESPECTIVE PLACES.

static constexpr VkCompareOp VulkanCompareOps[] = {
    VK_COMPARE_OP_NEVER,
    VK_COMPARE_OP_LESS,
    VK_COMPARE_OP_EQUAL,
    VK_COMPARE_OP_LESS_OR_EQUAL,
    VK_COMPARE_OP_GREATER,
    VK_COMPARE_OP_NOT_EQUAL,
    VK_COMPARE_OP_GREATER_OR_EQUAL,
    VK_COMPARE_OP_ALWAYS
};

static constexpr VkFormat VulkanFormats[] = {
    VK_FORMAT_R32G32B32_SFLOAT,
    VK_FORMAT_R32G32_SFLOAT,
    VK_FORMAT_B8G8R8A8_SRGB,
    VK_FORMAT_D32_SFLOAT_S8_UINT,
    VK_FORMAT_D24_UNORM_S8_UINT
};

static constexpr VkPolygonMode VulkanPolygonModes[] = {
    VK_POLYGON_MODE_POINT,
    VK_POLYGON_MODE_LINE,
    VK_POLYGON_MODE_FILL
};

static constexpr VkCullModeFlags VulkanCullModes[] = {
    VK_CULL_MODE_NONE,
    VK_CULL_MODE_FRONT_BIT,
    VK_CULL_MODE_BACK_BIT,
    VK_CULL_MODE_FRONT_AND_BACK
};

static constexpr VkFrontFace VulkanFrontFace[] = {
    VK_FRONT_FACE_COUNTER_CLOCKWISE,
    VK_FRONT_FACE_CLOCKWISE
};

static constexpr VkCommandPoolCreateFlags VulkanCommandPoolCreateFlags[] = {
    VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
    VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
    VK_COMMAND_POOL_CREATE_PROTECTED_BIT
};

static constexpr VkQueueFlags VulkanQueueFlags[] = {
    VK_QUEUE_GRAPHICS_BIT,
    VK_QUEUE_COMPUTE_BIT,
    VK_QUEUE_TRANSFER_BIT
};

static constexpr  VkCommandBufferLevel VulkanCommandBufferLevel[] = {
    VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    VK_COMMAND_BUFFER_LEVEL_SECONDARY
};

static VkPipelineStageFlags2 VulkanStageFlags(StageFlagBits flagsS) {

    uint32 flag = Underlying(flagsS);

    VkPipelineStageFlags2 flags = VK_PIPELINE_STAGE_NONE;

    if (flag & Underlying(StageFlagBits::STAGE_TOP_OF_PIPE)) flags |= VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT;
    if (flag & Underlying(StageFlagBits::STAGE_DRAW_INDIRECT)) flags |= VK_PIPELINE_STAGE_2_DRAW_INDIRECT_BIT;
    if (flag & Underlying(StageFlagBits::STAGE_VERTEX_INPUT)) flags |= VK_PIPELINE_STAGE_2_VERTEX_INPUT_BIT;
    if (flag & Underlying(StageFlagBits::STAGE_VERTEX_SHADER)) flags |= VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT;
    if (flag & Underlying(StageFlagBits::STAGE_FRAGMENT_SHADER)) flags |= VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT;
    if (flag & Underlying(StageFlagBits::STAGE_COLOUR_ATTACHMENT_OUTPUT)) flags |= VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
    if (flag & Underlying(StageFlagBits::STAGE_COMPUTE_SHADER)) flags |= VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    if (flag & Underlying(StageFlagBits::STAGE_ALL_COMMANDS)) flags |= VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;

    return flags;
}

static VkAccessFlags2 VulkanAccessFlags(AccessMaskFlagBits flagAM) {

    uint32 flag = Underlying(flagAM);
    VkAccessFlags2 flags = VK_ACCESS_2_NONE;

    if (flag & Underlying(AccessMaskFlagBits::ACCESS_INDIRECT_COMMAND_READ)) flags |= VK_ACCESS_2_INDIRECT_COMMAND_READ_BIT;
    if (flag & Underlying(AccessMaskFlagBits::ACCESS_INDEX_READ)) flags |= VK_ACCESS_2_INDEX_READ_BIT;
    if (flag & Underlying(AccessMaskFlagBits::ACCESS_VERTEX_ATTRIBUTE_READ)) flags |= VK_ACCESS_2_VERTEX_ATTRIBUTE_READ_BIT;
    if (flag & Underlying(AccessMaskFlagBits::ACCESS_UNIFORM_READ)) flags |= VK_ACCESS_2_UNIFORM_READ_BIT;
    if (flag & Underlying(AccessMaskFlagBits::ACCESS_INPUT_ATTACHMENT_READ)) flags |= VK_ACCESS_2_INPUT_ATTACHMENT_READ_BIT;
    if (flag & Underlying(AccessMaskFlagBits::ACCESS_SHADER_READ)) flags |= VK_ACCESS_2_SHADER_READ_BIT;
    if (flag & Underlying(AccessMaskFlagBits::ACCESS_SHADER_WRITE)) flags |= VK_ACCESS_2_SHADER_WRITE_BIT;
    if (flag & Underlying(AccessMaskFlagBits::ACCESS_COLOUR_ATTACHMENT_READ)) flags |= VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT;
    if (flag & Underlying(AccessMaskFlagBits::ACCESS_COLOUR_ATTACHMENT_WRITE)) flags |= VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
    if (flag & Underlying(AccessMaskFlagBits::ACCESS_DEPTH_STENCIL_ATTACHMENT_READ)) flags |= VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
    if (flag & Underlying(AccessMaskFlagBits::ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE)) flags |= VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    if (flag & Underlying(AccessMaskFlagBits::ACCESS_TRANSFER_READ)) flags |= VK_ACCESS_2_TRANSFER_READ_BIT;
    if (flag & Underlying(AccessMaskFlagBits::ACCESS_TRANSFER_WRITE)) flags |= VK_ACCESS_2_TRANSFER_WRITE_BIT;
    if (flag & Underlying(AccessMaskFlagBits::ACCESS_MEMORY_READ)) flags |= VK_ACCESS_2_MEMORY_READ_BIT;
    if (flag & Underlying(AccessMaskFlagBits::ACCESS_MEMORY_WRITE)) flags |= VK_ACCESS_2_MEMORY_WRITE_BIT;
    return flags;
}

static constexpr VkImageLayout VulkanTransitionFlags[] = {
    VK_IMAGE_LAYOUT_UNDEFINED,
    VK_IMAGE_LAYOUT_GENERAL,
    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
    VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    // VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
    // VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    // VK_IMAGE_LAYOUT_PREINITIALIZED
};

static VkImageAspectFlags VulkanImageAspectFlags(ImageAspectFlagBits flagsIA) {
    uint32 flag = Underlying<ImageAspectFlagBits>(flagsIA);

    VkImageAspectFlags flags = VK_IMAGE_ASPECT_NONE;

    if (flag & Underlying(ImageAspectFlagBits::ASPECT_COLOUR_BIT))  flags |= VK_IMAGE_ASPECT_COLOR_BIT;
    if (flag & Underlying(ImageAspectFlagBits::ASPECT_DEPTH_BIT))   flags |= VK_IMAGE_ASPECT_DEPTH_BIT;
    if (flag & Underlying(ImageAspectFlagBits::ASPECT_STENCIL_BIT)) flags |= VK_IMAGE_ASPECT_STENCIL_BIT;
    if (flag & Underlying(ImageAspectFlagBits::ASPECT_METADATA_BIT)) flags |= VK_IMAGE_ASPECT_METADATA_BIT;

    return flags;
}




#endif //ALKYONERENDERENGINE_VULKANDEFINITIONS_H