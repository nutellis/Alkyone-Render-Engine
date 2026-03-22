//
// Created by Nutellis on 19-Feb-26.
//

#ifndef ALKYONERENDERENGINE_IFRAMESYNC_H
#define ALKYONERENDERENGINE_IFRAMESYNC_H

#include <core/PODTypes.h>

//TODO: move this to an options file and make it variable (double/tripple buffering)
static constexpr unsigned int FRAMES_IN_FLIGHT = 2;

class IFrameSync
{
public:
    IFrameSync() = default;
    virtual ~IFrameSync() = default;

    virtual bool Initialize() = 0;
    virtual void Terminate() = 0;
    virtual uint32 BeginSynchronize() = 0;
    virtual void ResetSyncObjects() = 0;
    virtual void EndSynchronize() = 0;
};


#endif //ALKYONERENDERENGINE_IFRAMESYNC_H