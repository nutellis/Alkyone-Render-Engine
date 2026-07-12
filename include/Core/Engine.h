//
// Created by Nutellis on 20-Jan-26.
//

#ifndef ALKYONERENDERENGINE_ENGINE_H
#define ALKYONERENDERENGINE_ENGINE_H
#include "rhi/core/DynamicRHI.h"
#include "scene/SceneManager.h"

#include "shaders/ShaderManager.h"

class ResourceManager;
class DynamicRHI;
class ARWindow;

class AlkyoneRenderEngine
{
    public:

    // the engine owns the window instance.

    // the API context

    // the actual renderer
private:
    ARWindow *window;

    DynamicRHI *rhi;
    ShaderManager *shaderManager;

    ResourceManager *resourceManager;
    SceneManager *sceneManager;

public:
    AlkyoneRenderEngine();
    ~AlkyoneRenderEngine();

    bool Initialize();
    void Terminate() const;

    void Run() const;

};

#endif //ALKYONERENDERENGINE_ENGINE_H