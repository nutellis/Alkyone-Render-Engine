//
// Created by Nutellis on 20-Feb-26.
//

#ifndef ALKYONERENDERENGINE_ITEXTURE_H
#define ALKYONERENDERENGINE_ITEXTURE_H

#include <core/PODTypes.h>

enum TextureUsage
{
    NONE = 0,
    COLOUR_ATTACHMENT = BIT(0),
    DEPTH_TARGET = BIT(1),
    STORAGE = BIT(2)
};

class ITexture
{
public:
    virtual ~ITexture() = default;

    // virtual uint64 GetWidth() const = 0;
    // virtual uint64 GetHeight() const = 0;
    // virtual uint64 GetDepth() const = 0;
    // virtual uint64 GetMipLevels() const = 0;
    // virtual uint64 GetArrayLayers() const = 0;
    // virtual TextureFormat GetFormat() const = 0;
};

#endif //ALKYONERENDERENGINE_ITEXTURE_H