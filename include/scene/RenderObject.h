//
// Created by Nutellis on 23-Jan-26.
//

#ifndef ALKYONERENDERENGINE_RENDERENTITY_H
#define ALKYONERENDERENGINE_RENDERENTITY_H

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
    // glm::mat4x3 transform = {};
    // glm::mat4x3 combined_transform = {};
    // std::optional<RenderEntityId> first_child = {};
    // std::optional<RenderEntityId> next_sibling = {};
    // std::optional<RenderEntityId> parent = {};
    // std::optional<u32> mesh_group_manifest_index = {};
    // ObjectType type = ObjectType::UNKNOWN;
    // std::string name = {};
    // std::optional<u32> light_index = {};
    bool dirty = {};
};


#endif //ALKYONERENDERENGINE_RENDERENTITY_H