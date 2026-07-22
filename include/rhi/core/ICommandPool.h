//
// Created by Nutellis on 11-Feb-26.
//

#ifndef ALKYONERENDERENGINE_ICOMMANDPOOL_H
#define ALKYONERENDERENGINE_ICOMMANDPOOL_H
#include "ICommandBuffer.h"


class ICommandPool
{
    public:
    virtual ~ICommandPool() = default;

    virtual bool Initialize() = 0;
    virtual void Terminate() = 0;

    virtual ICommandBuffer * AllocateCommandBuffer(CommandBufferLevel bufferLevel) = 0;

};


#endif //ALKYONERENDERENGINE_ICOMMANDPOOL_H