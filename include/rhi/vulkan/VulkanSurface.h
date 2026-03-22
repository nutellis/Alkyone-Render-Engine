//
// Created by Nutellis on 14-Feb-26.
//

#ifndef ALKYONERENDERENGINE_VULKANSURFACE_H
#define ALKYONERENDERENGINE_VULKANSURFACE_H

#include "rhi/core/ISurface.h"

#include <volk.h>

#include "core/ApplicationWindow.h"

class VulkanDevice;
class ARWindow;

class VulkanSurface: public ISurface
{
    public:
    VulkanSurface();
    ~VulkanSurface() override;

    bool Initialize(ARWindow* window);
    void Terminate();

private:
    VulkanDevice *parent;

};


#endif //ALKYONERENDERENGINE_VULKANSURFACE_H