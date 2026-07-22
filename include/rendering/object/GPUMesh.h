//
// Created by Nutellis on 24-Jun-26.
//

#ifndef ALKYONERENDERENGINE_GPUMESH_H
#define ALKYONERENDERENGINE_GPUMESH_H

#include <core/PODTypes.h>

struct GPUMesh
{
    uint32 firstIndex{};
    uint32 indexCount{};
    uint32 vertexOffset{};
    uint32 indexBufferOffset{};
    uint32 materialIndex{};
    Handle bufferHandle;
};

#endif //ALKYONERENDERENGINE_GPUMESH_H
