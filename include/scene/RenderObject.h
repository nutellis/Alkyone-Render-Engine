//
// Created by Nutellis on 23-Jan-26.
//

#ifndef ALKYONERENDERENGINE_RENDERENTITY_H
#define ALKYONERENDERENGINE_RENDERENTITY_H
#include <string>

#include "containers/SlotMap.h"

class MeshGroup;


enum struct ObjectType
{
    ROOT,
    TRANSFORM,
    POINT_LIGHT,
    SPOT_LIGHT,
    CAMERA,
    MESHGROUP,
    UNKNOWN
};

struct RenderObject
{
    using RenderObjectId = SlotMap<RenderObject>::ElementId;

    // glm::mat4x3 transform = {};
    // glm::mat4x3 combined_transform = {};
    // std::optional<RenderEntityId> first_child = {};
    RenderObjectId nextSibling = {};
    RenderObjectId parent = {};
    // uint32 mesh_group_index = {};
    ObjectType type = ObjectType::UNKNOWN;
    std::string name = {};
    // std::optional<u32> light_index = {};
    bool dirty = {};
};


#endif //ALKYONERENDERENGINE_RENDERENTITY_H