//
// Created by nutellis on 16/07/2026.
//

#ifndef ALKYONERENDERENGINE_SHADEROFFSET_H
#define ALKYONERENDERENGINE_SHADEROFFSET_H
#include "core/PODTypes.h"


struct ShaderOffset {
    uint32 uniformOffset = 0;
    uint32 bindingRangeIndex = 0;
    uint32 bindingArrayIndex = 0;
};


#endif //ALKYONERENDERENGINE_SHADEROFFSET_H