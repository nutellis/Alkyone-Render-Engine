//
// Created by Nutellis on 13-Feb-26.
//

#ifndef ALKYONERENDERENGINE_IDEVICE_H
#define ALKYONERENDERENGINE_IDEVICE_H

class IDevice
{
    public:
    IDevice() = default;
    virtual ~IDevice() = default;

    virtual bool Initialize() = 0;
    virtual void Terminate() = 0;

    virtual void WaitIdle() = 0;

};
#endif //ALKYONERENDERENGINE_IDEVICE_H