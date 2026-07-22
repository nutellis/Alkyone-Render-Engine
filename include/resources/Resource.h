//
// Created by Nutellis on 13-Jun-26.
//

#ifndef ALKYONERENDERENGINE_RESOURCE_H
#define ALKYONERENDERENGINE_RESOURCE_H

#include "ResourceHandle.h"
#include "core/PODTypes.h"

enum struct ResourceType : uint32
{
    Texture,
    Mesh,
    Scene,
    Material,
    Shader,
    Unknown
};



template<typename T>
class Resource
{
private:
    ResourceHandle<T> resourceId;
    bool isLoaded = false;

public:
    explicit Resource(ResourceHandle<T> resourceId): resourceId(resourceId) {};
    virtual ~Resource() = default;

    const uint32& GetResourceId() const {
        return resourceId;
    }

    bool IsLoaded() const
    {
        return isLoaded;
    }

    bool Load()
    {
        isLoaded = DoLoad();
        return isLoaded;
    }
    bool Unload()
    {
        DoUnload();
        isLoaded = false;
    }

protected:
    virtual bool DoLoad() = 0;
    virtual bool DoUnload() = 0;
};
#endif //ALKYONERENDERENGINE_RESOURCE_H