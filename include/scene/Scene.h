//
// Created by Nutellis on 25-Jan-26.
//

#ifndef ALKYONERENDERENGINE_SCENE_H
#define ALKYONERENDERENGINE_SCENE_H

#include "RenderObject.h"
#include "containers/SlotMap.h"
#include "rendering/Mesh.h"


class Scene
{
    using RenderObjectId = SlotMap<RenderObject>::ElementId;

    Scene() = default;
    ~Scene() = default;

    void AddRenderObject(RenderObject& object);
    void RemoveRenderObject(RenderObject& object);

    void Update();
    void LoadScene(std::string filepath);
    void LoadObject();


    SlotMap<RenderObject> renderObjects; // every new object here
    std::vector<RenderObjectId> rootObjects; // a list of all objects without a direct parent(on the root level)

};


#endif //ALKYONERENDERENGINE_SCENE_H