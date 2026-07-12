//
// Created by Nutellis on 13-Feb-26.
//

#ifndef ALKYONERENDERENGINE_RENDERER_H
#define ALKYONERENDERENGINE_RENDERER_H
#include "containers/SlotMap.h"


class ResourceManager;
class Mesh;

class Renderer
{
public:
    Renderer() = delete;
    explicit Renderer(ResourceManager& resourceManager);
    ~Renderer();

    bool Initialize();
    void Terminate();


private:
    ResourceManager& resourceManager;

    std::vector<Mesh> allMeshes;


};


#endif //ALKYONERENDERENGINE_RENDERER_H