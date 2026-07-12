//
// Created by Nutellis on 11-Feb-26.
//

#ifndef ALKYONERENDERENGINE_ICOMMANDBUFFER_H
#define ALKYONERENDERENGINE_ICOMMANDBUFFER_H
#include <vector>

#include "barriers/ImageBarrier.h"


enum CommandBufferLevel
{
    COMMAND_BUFFER_LEVEL_PRIMARY = 0, // VK_COMMAND_BUFFER_LEVEL_PRIMARY | D3D12_COMMAND_LIST_TYPE_DIRECT
    COMMAND_BUFFER_LEVEL_SECONDARY = 1 // VK_COMMAND_BUFFER_LEVEL_SECONDARY | D3D12_COMMAND_LIST_TYPE_BUNDLE
};

struct CopyRequest
{
    Handle src;
    Handle dst;
    struct CopyRegion
    {
        uint64 srcOffset;
        uint64 dstOffset;
        uint64 size;
    } region;
};


class ICommandBuffer
{
public:
    ICommandBuffer() = default;
    virtual ~ICommandBuffer() = default;

    virtual bool Initialize() = 0;
    virtual void Terminate() = 0;

    virtual void Begin() = 0;
    virtual void Reset() = 0;

    virtual void Barrier(ImageBarrier barrier) = 0;
};



#endif //ALKYONERENDERENGINE_ICOMMANDBUFFER_H