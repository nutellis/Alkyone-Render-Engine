//
// Created by Nutellis on 20-Jan-26.
//

#include "rhi/IGraphicsContext.h"

#include "../../include/rhi/vulkan/VulkanContext.h"
#include "GLFW/glfw3.h"


IGraphicsContext* IGraphicsContext::CreateContext(GLFWwindow * windowHandle, RendererBackend rendererBackend)
{
    IGraphicsContext * context = nullptr;

    switch (rendererBackend)
    {
    case RendererBackend::OpenGL:
        //context = std::make_unique<OpenGLContext>(window);
        break;
    case RendererBackend::Vulkan:
        context = new VulkanContext(windowHandle);
        break;
    case RendererBackend::DirectX:
        break;
    }

    if (context) {
        context->Initialize();
    }

    return context;
}
