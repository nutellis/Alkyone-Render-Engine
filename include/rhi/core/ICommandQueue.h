//
// Created by Nutellis on 13-Feb-26.
//

#ifndef ALKYONERENDERENGINE_ICOMMANDQUEUE_H
#define ALKYONERENDERENGINE_ICOMMANDQUEUE_H
#include "core/PODTypes.h"

class IFrameSync;
class ICommandBuffer;

enum CommandQueueType
{
    GRAPHICS_QUEUE = 0,
    COMPUTE_QUEUE,
    TRANSFER_QUEUE
};

class ICommandQueue
{
public:
    ICommandQueue() = default;
    virtual ~ICommandQueue() = default;

    virtual bool Initialize() = 0;
    virtual void Terminate() = 0;

    virtual void AllocateCommandBuffers(uint32 count) = 0;
    virtual void SubmitCommandBuffer(IFrameSync& sync, ICommandBuffer& cmdBuffer) = 0;
};


#endif //ALKYONERENDERENGINE_ICOMMANDQUEUE_H