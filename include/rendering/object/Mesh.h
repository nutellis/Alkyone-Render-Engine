//
// Created by Nutellis on 23-Jan-26.
//

#ifndef ALKYONERENDERENGINE_MESH_H
#define ALKYONERENDERENGINE_MESH_H

#include "core/PODTypes.h"

struct Mesh
{
public:
    uint32 firstIndex{};
    uint32 indexCount{};
    uint32 vertexOffset{};
    uint32 indexBufferOffset{};
    uint32 materialIndex{};

    //bool isRenderable; // make it a bitfield

};
#endif //ALKYONERENDERENGINE_MESH_H