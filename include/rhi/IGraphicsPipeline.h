//
// Created by Nutellis on 30-Jan-26.
//

#ifndef ALKYONERENDERENGINE_IPIPELINE_H
#define ALKYONERENDERENGINE_IPIPELINE_H
#include "GraphicsPipelineDesc.h"


class IGraphicsPipeline
{
public:
    IGraphicsPipeline() = default;
    virtual ~IGraphicsPipeline() = default;

    virtual void CreatePipeline(GraphicsPipelineDesc desc)= 0;
    virtual void DestroyPipeline()= 0;

    virtual void BindPipeline() = 0;
};
#endif //ALKYONERENDERENGINE_IPIPELINE_H