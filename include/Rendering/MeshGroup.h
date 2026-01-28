//
// Created by Nutellis on 23-Jan-26.
//

#ifndef ALKYONERENDERENGINE_MESHGROUP_H
#define ALKYONERENDERENGINE_MESHGROUP_H

#include "containers/Array.h"


class MeshLodGroup;
class Mesh;

class MeshGroup
{
    // a collection of all the meshes (and their LODs) that make up the object.
public:
    TArray<Mesh *> meshLodGroups;

public:
    MeshGroup();
    ~MeshGroup();

};


#endif //ALKYONERENDERENGINE_MESHGROUP_H