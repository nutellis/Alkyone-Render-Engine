//
// Created by Nutellis on 23-Jan-26.
//

#ifndef ALKYONERENDERENGINE_MESHLODGROUP_H
#define ALKYONERENDERENGINE_MESHLODGROUP_H
#include "containers/DArray.h"


class Mesh;

class MeshLodGroup
{
    // the actual LODs of the mesh. Zero is the default.
    DArray<Mesh> meshLods;
};


#endif //ALKYONERENDERENGINE_MESHLODGROUP_H