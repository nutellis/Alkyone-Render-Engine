//
// Created by Nutellis on 23-Jan-26.
//

#include <rendering/MeshGroup.h>

#include <rendering/MeshLodGroup.h>

#include "rendering/Mesh.h"

MeshGroup::MeshGroup()
{
    meshLodGroups = DArray<Mesh*>();
}

MeshGroup::~MeshGroup()
{

}

void MeshGroup::Terminate()
{
    for (Mesh* mesh : meshLodGroups)
    {
        mesh->Terminate();
    }
}
