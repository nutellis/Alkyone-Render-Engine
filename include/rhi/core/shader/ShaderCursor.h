//
// Created by nutellis on 16/07/2026.
//

#ifndef ALKYONERENDERENGINE_SHADERCURSOR_H
#define ALKYONERENDERENGINE_SHADERCURSOR_H

#include <slang.h>
#include <string>

#include "ShaderOffset.h"
#include "math/Types.h"
#include "math/Vector3.h"


class IShaderObject;

class ShaderCursor {
public:
    ShaderCursor field(const char *name);
    ShaderCursor field(int32 index);
    ShaderCursor element(int32 index);


    //write functions
   // void write(Texture* texture);
    //void write(Sampler* sampler);
    void write(const void* data, size_t size);
    void write(Float3 value) { write(&value, sizeof(value)); }


private:
    IShaderObject * baseObject = nullptr;
    ShaderOffset offset;

    slang::TypeLayoutReflection* typeLayout{};

};


#endif //ALKYONERENDERENGINE_SHADERCURSOR_H