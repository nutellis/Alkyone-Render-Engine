//
// Created by Nutellis on 23-Jan-26.
//

#ifndef ALKYONERENDERENGINE_MESH_H
#define ALKYONERENDERENGINE_MESH_H

#include <containers/Array.h>

struct Vertex;

class Mesh
{
public:
    TArray<Vertex> vertices;
    TArray<unsigned int> indices;

    size_t firstIndex;
    size_t indexCount;
    size_t materialIndex;

};


#endif //ALKYONERENDERENGINE_MESH_H