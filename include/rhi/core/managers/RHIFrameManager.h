//
// Created by nutellis on 29/07/2026.
//

#ifndef ALKYONERENDERENGINE_FRAMEMANAGER_H
#define ALKYONERENDERENGINE_FRAMEMANAGER_H
#include "core/PODTypes.h"


class RHIFrameManager {
public:
    RHIFrameManager() = default;
    virtual ~RHIFrameManager() = 0;

    virtual bool Initialize() = 0;
    virtual void Terminate() = 0;

    virtual bool BeginSynchronize() = 0;
    virtual void SubmitQueue()  = 0;
    virtual void Present()  = 0;
    virtual void ResetSyncObjects() = 0;
    virtual void EndSynchronize() = 0;

    uint32 currentFrame = 0;
    uint32 imageIndex = 0; //swapchain image index

};


#endif //ALKYONERENDERENGINE_FRAMEMANAGER_H