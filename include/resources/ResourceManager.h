//
// Created by Nutellis on 13-Jun-26.
//

#ifndef ALKYONERENDERENGINE_RESOURCEMANAGER_H
#define ALKYONERENDERENGINE_RESOURCEMANAGER_H

#include <span>
#include <string>
#include <unordered_map>
#include "Resource.h"
#include <containers/SlotMap.h>

#include "GLTFImporter.h"
#include "rendering/types/Vertex.h"
#include "rhi/core/DynamicRHI.h"

/*
* It starts with external code asking for a particular resource, like the coin texture.
* The resource manager will check if it has a copy of the resource in its cache.
* If it does, then it returns a handle to that copy.
* Otherwise, it loads the resource from disk, adds it to the cache, and returns a pointer to the calling code.
* The resource manager will also deallocate resources once they’re no longer needed
* (either immediately, or when instructed to do so).
*/

class DynamicRHI;
struct MeshGroup;

class ResourceManager
{

public:
    ResourceManager() = delete;

    explicit ResourceManager(DynamicRHI& rhi);
    ~ResourceManager();

    bool Initialize();
    void Terminate();

    void LoadSceneGeometry(ParsedData &parsedData);

    void UploadMeshData(std::span<const Vertex> vertices, std::span<const uint32> indices);

    ShaderHandle LoadShader(const std::string &filename, const std::string &vertexEntryName,
                            const std::string &fragmentEntryName);


private:
    DynamicRHI& rhi;

    std::unordered_map<std::string, ResourceHandle<void>> resourceCache;
    std::unordered_map<std::string, ShaderHandle> shaderCache;

    SlotMap<MeshGroup> meshRegistry;
    BufferHandle megaBufferHandle;

    //SlotMap<Scene> SceneRegistry;
    //SlotMap<Texture> textureRegistry;
    //SlotMap<Material> materialRegistry;

    uint32 refCount = 0;

};


#endif //ALKYONERENDERENGINE_RESOURCEMANAGER_H