//
// Created by Nutellis on 13-Jun-26.
//

#ifndef ALKYONERENDERENGINE_RESOURCEMANAGER_H
#define ALKYONERENDERENGINE_RESOURCEMANAGER_H

#include <string>
#include <unordered_map>
#include <any>
#include <ctime>
#include "Resource.h"
#include "mesh/GLTFImporter.h"
#include "scene/Scene.h"

/*
* It starts with external code asking for a particular resource, like the coin texture.
* The resource manager will check if it has a copy of the resource in its cache.
* If it does, then it returns a handle to that copy.
* Otherwise, it loads the resource from disk, adds it to the cache, and returns a pointer to the calling code.
* The resource manager will also deallocate resources once they’re no longer needed
* (either immediately, or when instructed to do so).
*/

class ResourceManager
{

private:
    std::unordered_map<std::string, ResourceHandle<void>> resourceCache;

    SlotMap<Mesh> meshRegistry;
    SlotMap<Scene> SceneRegistry;
    //SlotMap<Texture> textureRegistry;
    SlotMap<Material> materialRegistry;

uint32 refCount = 0;

public:
    template<typename T>
    ResourceHandle<T> GetResource(const std::string& resourcePath, ResourceType resourceType)
    {
        //we check if the resource exists
        if (resourceCache.contains(resourcePath))
        {
            return std::any_cast<ResourceHandle<T>>(resourceCache[resourcePath]);
        }
    }
};


#endif //ALKYONERENDERENGINE_RESOURCEMANAGER_H