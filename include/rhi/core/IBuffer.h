//
// Created by Nutellis on 08-Mar-26.
//

#ifndef ALKYONERENDERENGINE_IBUFFER_H
#define ALKYONERENDERENGINE_IBUFFER_H
#include "rhi/descriptors/BufferDesc.h"

class IBuffer
{
public:
    IBuffer() = default;
    virtual ~IBuffer() = default;

    virtual bool Initialize(BufferDesc desc) = 0;
    virtual void Terminate() = 0;
    virtual void Map() = 0;
    virtual void Unmap() = 0;
};


#endif //ALKYONERENDERENGINE_IBUFFER_H