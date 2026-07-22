//
// Created by Nutellis on 25-Jan-26.
//

#include <scene/Scene.h>

#include "resources/GLTFImporter.h"

SceneObjectHandle Scene::AddSceneObject(SceneObject& object)
{
    // renderObjects.push_back(object);
    // Handle id = Handle(1, static_cast<uint32>(renderObjects.size() - 1));
    return SceneObjectHandle{Handle(1, static_cast<uint32>(objects.size()))};
}

void Scene::RemoveRenderObject(SceneObjectHandle renderObjectHandle)
{
}

void Scene::Update()
{
}

