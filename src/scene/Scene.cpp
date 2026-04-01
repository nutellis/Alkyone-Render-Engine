//
// Created by Nutellis on 25-Jan-26.
//

#include <scene/Scene.h>

#include "utilities/GLTFLoader.h"

void Scene::AddRenderObject(RenderObject& object)
{
}

void Scene::RemoveRenderObject(RenderObject& object)
{
}

void Scene::Update()
{
}

void Scene::LoadScene(const std::string filepath)
{
    GLTFLoader::LoadScene(filepath);
}


void Scene::LoadObject()
{

}
