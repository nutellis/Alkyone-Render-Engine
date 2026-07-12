//
// Created by Nutellis on 13-Jun-26.
//

#ifndef ALKYONERENDERENGINE_RESOURCEMANAGER_H
#define ALKYONERENDERENGINE_RESOURCEMANAGER_H

#include <string>
#include <unordered_map>
#include "Resource.h"
#include <containers/SlotMap.h>

/*
* It starts with external code asking for a particular resource, like the coin texture.
* The resource manager will check if it has a copy of the resource in its cache.
* If it does, then it returns a handle to that copy.
* Otherwise, it loads the resource from disk, adds it to the cache, and returns a pointer to the calling code.
* The resource manager will also deallocate resources once they’re no longer needed
* (either immediately, or when instructed to do so).
*/

class DynamicRHI;
class MeshGroup;
struct GPUMesh;

class ResourceManager
{

public:
    ResourceManager() = delete;

    explicit ResourceManager(DynamicRHI& rhi);
    ~ResourceManager();

    bool Initialize();
    void Terminate();

    void UploadMeshes(std::vector<MeshGroup>& meshGroups);
    


private:
    DynamicRHI& rhi;

    std::unordered_map<std::string, ResourceHandle<void>> resourceCache;

    SlotMap<GPUMesh> meshRegistry;
    //SlotMap<Scene> SceneRegistry;
    //SlotMap<Texture> textureRegistry;
    //SlotMap<Material> materialRegistry;

    uint32 refCount = 0;

};


#endif //ALKYONERENDERENGINE_RESOURCEMANAGER_H