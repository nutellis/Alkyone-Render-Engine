//
// Created by Nutellis on 25-Jan-26.
//

#include <scene/Scene.h>

void Scene::AddMesh(Mesh& mesh)
{
    meshObjects.createSlot(mesh);
}
