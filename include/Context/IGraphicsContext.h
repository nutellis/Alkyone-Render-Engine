//
// Created by Nutellis on 20-Jan-26.
//

#ifndef ALKYONERENDERENGINE_GRAPHICSCONTEXT_H
#define ALKYONERENDERENGINE_GRAPHICSCONTEXT_H

struct GLFWwindow;

enum class RendererBackend {
    OpenGL,
    Vulkan,
    DirectX
};

class IGraphicsContext
{
public:
    virtual ~IGraphicsContext() = default;

    virtual bool Initialize() = 0;
    virtual void Terminate() = 0;
    virtual void SwapBuffers() = 0;

    static IGraphicsContext* CreateContext(GLFWwindow * windowHandle, RendererBackend rendererBackend);
};


#endif //ALKYONERENDERENGINE_GRAPHICSCONTEXT_H