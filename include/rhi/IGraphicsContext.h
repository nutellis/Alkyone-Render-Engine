//
// Created by Nutellis on 20-Jan-26.
//

#ifndef ALKYONERENDERENGINE_GRAPHICSCONTEXT_H
#define ALKYONERENDERENGINE_GRAPHICSCONTEXT_H

#include <string>

#include "slang.h"

struct GLFWwindow;

enum class RendererBackend {
    OpenGL,
    Vulkan,
    DirectX
};

struct ContextSlangTargetOptions
{
    SlangCompileTarget format;
    std::string profile;
    slang::CompilerOptionName name;
    slang::CompilerOptionValue value;

};

class IGraphicsContext
{
public:
    virtual ~IGraphicsContext() = default;

    virtual bool Initialize() = 0;
    virtual void Terminate() = 0;
    virtual void SwapBuffers() = 0;

    virtual std::string GetBackendString() = 0;

    static IGraphicsContext* CreateContext(GLFWwindow * windowHandle, RendererBackend rendererBackend);

public :
    RendererBackend backend;
    ContextSlangTargetOptions slangTargetOptions {};
};


#endif //ALKYONERENDERENGINE_GRAPHICSCONTEXT_H