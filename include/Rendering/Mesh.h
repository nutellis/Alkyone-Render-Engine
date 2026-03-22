//
// Created by Nutellis on 23-Jan-26.
//

#ifndef ALKYONERENDERENGINE_MESH_H
#define ALKYONERENDERENGINE_MESH_H

#include <vector>

#include "core/PODTypes.h"

class IBuffer;
struct Vertex;

class Mesh
{
public:
    Mesh();
    ~Mesh();

    bool Initialize();
    void Terminate();

public:
    std::vector<Vertex> vertices;
    std::vector<uint32> indices; // consider uint16

    size_t firstIndex;
    size_t indexCount;
    size_t materialIndex;

    IBuffer * buffer;
};


#endif //ALKYONERENDERENGINE_MESH_H