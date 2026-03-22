//
// Created by Nutellis on 19-Feb-26.
//

#ifndef ALKYONERENDERENGINE_IMAGEBARRIER_H
#define ALKYONERENDERENGINE_IMAGEBARRIER_H

#include <core/PODTypes.h>
#include <rhi/core/RHIDefinitions.h>

class ITexture;

struct ImageBarrier
{
    StageFlagBits sourceStageMask = StageFlagBits::STAGE_NONE;
    AccessMaskFlagBits sourceAccessMask = AccessMaskFlagBits::ACCESS_NONE;

    StageFlagBits destStageMask = StageFlagBits::STAGE_NONE;
    AccessMaskFlagBits destAccessMask = AccessMaskFlagBits::ACCESS_NONE;

    TransitionFlags sourceState = STATE_UNDEFINED;
    TransitionFlags destState = STATE_UNDEFINED;

    uint32 srcQueueFamilyIndex{};
    uint32 dstQueueFamilyIndex{};

    ITexture * image{};

    //subresource
    ImageAspectFlagBits aspectMask = ImageAspectFlagBits::ASPECT_NONE;
    uint32 baseMipLevel = 0;
    uint32 levelCount = 1;
    uint32 baseArrayLayer = 0;
    uint32 layerCount = 1;

};
#endif //ALKYONERENDERENGINE_IMAGEBARRIER_H