//
// Created by Nutellis on 25-Jan-26.
//

#include <scene/Scene.h>

#include "../../include/resources/mesh/GLTFImporter.h"

RenderObjectHandle Scene::AddRenderObject(RenderObject& object)
{
    renderObjects.push_back(object);
    Handle id = Handle(1, static_cast<uint32>(renderObjects.size() - 1));
    return RenderObjectHandle{id};
}

void Scene::RemoveRenderObject(RenderObjectHandle renderObjectHandle)
{
}

void Scene::Update()
{
}

