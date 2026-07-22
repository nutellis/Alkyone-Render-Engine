//
// Created by Nutellis on 13-Jun-26.
//

#ifndef ALKYONERENDERENGINE_RESOURCEHANDLE_H
#define ALKYONERENDERENGINE_RESOURCEHANDLE_H
#include "core/PODTypes.h"

struct RHIShader;
struct RHIGraphicsPipeline;

template<typename T>
struct ResourceHandle
{
    Handle resourceId;

    explicit operator bool() const
{
    return resourceId != Handle::Invalid();
}
};

using ShaderHandle = ResourceHandle<RHIShader>;
using PipelineHandle = ResourceHandle<RHIGraphicsPipeline>;

#endif //ALKYONERENDERENGINE_RESOURCEHANDLE_H