//
// Created by Nutellis on 24-Mar-26.
//

#include "../../include/resources/AssetRegistry.h"

#include <rendering/Mesh.h>

#include "../../include/resources/mesh/GLTFImporter.h"

AssetRegistry::AssetRegistry()
{
}

AssetRegistry::~AssetRegistry()
{
}

bool AssetRegistry::Initialize()
{
    return true;
}

void AssetRegistry::Terminate()
{
}

void AssetRegistry::AddMesh(Mesh& mesh)
{
    meshes.createSlot(std::move(mesh));
}