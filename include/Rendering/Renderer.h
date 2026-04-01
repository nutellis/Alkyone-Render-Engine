//
// Created by Nutellis on 13-Feb-26.
//

#ifndef ALKYONERENDERENGINE_RENDERER_H
#define ALKYONERENDERENGINE_RENDERER_H
#include "containers/SlotMap.h"


class GPUMesh;

class Renderer
{
public:
    SlotMap<GPUMesh> meshes;

    //materials - pipelines

};


#endif //ALKYONERENDERENGINE_RENDERER_H