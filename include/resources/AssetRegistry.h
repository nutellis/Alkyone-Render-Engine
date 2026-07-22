//
// Created by Nutellis on 24-Mar-26.
//

#ifndef ALKYONERENDERENGINE_ASSETREGISTRY_H
#define ALKYONERENDERENGINE_ASSETREGISTRY_H
#include "containers/SlotMap.h"


class GLTFImporter;
struct Mesh;

class AssetRegistry
{
public:
    AssetRegistry();
    ~AssetRegistry();

    bool Initialize();
    void Terminate();

public:
    SlotMap<Mesh> meshes; //change this to MeshGroup in the future


    void AddMesh(Mesh& mesh);
};


#endif //ALKYONERENDERENGINE_ASSETREGISTRY_H