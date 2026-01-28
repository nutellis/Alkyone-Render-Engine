//
// Created by Nutellis on 20-Jan-26.
//

#ifndef ALKYONERENDERENGINE_ENGINE_H
#define ALKYONERENDERENGINE_ENGINE_H

class ARWindow;

class AlkyoneRenderEngine
{
    public:

    // the engine owns the window instance.

    // the API context

    // the actual renderer
private:
    ARWindow *window;

public:
    AlkyoneRenderEngine();
    ~AlkyoneRenderEngine();

    bool Initialize();
    void Terminate();

    void Run();
};

#endif //ALKYONERENDERENGINE_ENGINE_H