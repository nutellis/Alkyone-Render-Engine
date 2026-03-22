//
// Created by Nutellis on 20-Jan-26.
//

#ifndef ALKYONERENDERENGINE_ENGINE_H
#define ALKYONERENDERENGINE_ENGINE_H
#include "rhi/core/DynamicRHI.h"

#include "shaders/ShaderManager.h"

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

public:
    AlkyoneRenderEngine();
    ~AlkyoneRenderEngine();

    bool Initialize();
    void Terminate();

    void Run();

};

#endif //ALKYONERENDERENGINE_ENGINE_H