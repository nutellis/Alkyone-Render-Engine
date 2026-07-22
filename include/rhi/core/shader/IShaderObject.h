//
// Created by nutellis on 16/07/2026.
//

#ifndef ALKYONERENDERENGINE_ISHADEROBJECT_H
#define ALKYONERENDERENGINE_ISHADEROBJECT_H
#include "ShaderOffset.h"


class IShaderObject {

public:
    IShaderObject() = default;
    virtual ~IShaderObject() = default;
    //virtual void write(ShaderOffset, Texture & texture) = 0;
    //virtual void write(ShaderOffset offset, Sampler* sampler) = 0;
    virtual void write(ShaderOffset offset, const void* data, size_t size) = 0;
};


#endif //ALKYONERENDERENGINE_ISHADEROBJECT_H