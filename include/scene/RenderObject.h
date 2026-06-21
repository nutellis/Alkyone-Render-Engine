//
// Created by Nutellis on 23-Jan-26.
//

#ifndef ALKYONERENDERENGINE_RENDERENTITY_H
#define ALKYONERENDERENGINE_RENDERENTITY_H
#include <string>

#include "containers/SlotMap.h"


class MeshGroup;


enum struct ObjectType : uint32
{
    ROOT,
    TRANSFORM,
    POINT_LIGHT,
    SPOT_LIGHT,
    DIRECTIONAL_LIGHT,
    CAMERA,
    MESHGROUP,
    UNKNOWN
};

struct RenderObjectHandle
{
    Handle id;
};

struct RenderObject
{

public:

    RenderObjectHandle firstChild = {
        Handle(1, 0xFFFFFFFF)
    };
    RenderObjectHandle nextSibling = {
        Handle(1, 0xFFFFFFFF)
    };
    RenderObjectHandle parent = {
        Handle(1, 0xFFFFFFFF)
    };

};




#endif //ALKYONERENDERENGINE_RENDERENTITY_H