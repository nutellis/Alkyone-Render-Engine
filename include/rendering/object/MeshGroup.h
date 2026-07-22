//
// Created by Nutellis on 23-Jan-26.
//

#ifndef ALKYONERENDERENGINE_MESHGROUP_H
#define ALKYONERENDERENGINE_MESHGROUP_H

#include <vector>
#include <rendering/object/Mesh.h>

class MeshLodGroup;

struct MeshGroup
{
    // a collection of all the meshes (and their LODs) that make up the object.
public:
    std::vector<Mesh> subMeshes;
};


#endif //ALKYONERENDERENGINE_MESHGROUP_H