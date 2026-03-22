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

    Scene() = default;
    ~Scene() = default;

    void AddMesh(Mesh& mesh);
    void RemoveMesh(Mesh& mesh);

    void AddRenderObject(RenderObject& object);
    void RemoveRenderObject(RenderObject& object);

    void Update();


    SlotMap<Mesh> meshObjects; //every new mesh is added here
    SlotMap<RenderObject> renderObjects; // every new object here

    std::vector<RenderObject> rootObjects; // a list of all objects without a direct parent(on the root level)

};


#endif //ALKYONERENDERENGINE_SCENE_H