//
// Created by Nutellis on 23-Jan-26.
//

#ifndef ALKYONERENDERENGINE_MESHGROUP_H
#define ALKYONERENDERENGINE_MESHGROUP_H

#include <vector>
#include <core/PODTypes.h>


class MeshLodGroup;
class Mesh;
struct Vertex;

class MeshGroup
{
    // a collection of all the meshes (and their LODs) that make up the object.
public:
    std::vector<Vertex> vertices;
    std::vector<uint32> indices; // consider uint16

    std::vector<Mesh> subMeshes;

    Handle bufferHandle;

public:
    MeshGroup();
    ~MeshGroup();

    bool Initialize();
    void Terminate();
};


#endif //ALKYONERENDERENGINE_MESHGROUP_H