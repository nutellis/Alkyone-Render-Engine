//
// Created by Nutellis on 23-Jan-26.
//

#ifndef ALKYONERENDERENGINE_RENDERENTITY_H
#define ALKYONERENDERENGINE_RENDERENTITY_H
#include <string>

#include "containers/SlotMap.h"
#include "math/Matrix4.h"

class MeshGroup;


enum struct ObjectType
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

struct RenderObject
{
    using RenderObjectId = SlotMap<RenderObject>::ElementId;

    Matrix4f transform = {}; //model matrix
    // glm::mat4x3 combined_transform = {};
    RenderObjectId firstChild = {};
    RenderObjectId nextSibling = {};
    RenderObjectId parent = {};

    uint32 meshGroupIndex = {};

    ObjectType type = ObjectType::UNKNOWN;

    std::string name = {};
    // std::optional<u32> light_index = {};
    bool dirty = {};
};


#endif //ALKYONERENDERENGINE_RENDERENTITY_H