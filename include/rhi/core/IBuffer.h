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

    virtual void CopyData(const void* data, uint64 size, uint64 offset = 0) = 0;

};


#endif //ALKYONERENDERENGINE_IBUFFER_H