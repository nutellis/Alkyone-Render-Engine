//
// Created by Nutellis on 13-Feb-26.
//

#ifndef ALKYONERENDERENGINE_ISWAPCHAIN_H
#define ALKYONERENDERENGINE_ISWAPCHAIN_H
#include "IFrameSync.h"

class ISwapChain
{
public:
    ISwapChain() = default;
    virtual ~ISwapChain() = default;

    virtual bool Initialize() = 0;
    virtual void Terminate() = 0;

    virtual void RecreateSwapChain() = 0;
    virtual void CleanupSwapChain() = 0;
    virtual void Present(IFrameSync& frameSync) = 0;
    [[nodiscard]] virtual bool IsFrameBufferResized() const = 0;
    virtual void ResetIsFrameBufferResized() const = 0;
};

#endif //ALKYONERENDERENGINE_ISWAPCHAIN_H