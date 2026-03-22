//
// Created by Nutellis on 20-Jan-26.
//

#include <rhi/core/DynamicRHI.h>

#include <rhi/vulkan/VulkanRHI.h>



DynamicRHI* DynamicRHI::CreateContext(ARWindow* window, RendererBackend rendererBackend)
{
    DynamicRHI * context = nullptr;

    switch (rendererBackend)
    {
    case RendererBackend::OpenGL:
        //context = std::make_unique<OpenGLContext>(window);
        break;
    case RendererBackend::Vulkan:
        context = new VulkanRHI(window);
        break;
    case RendererBackend::D3D12:
        break;
    }

    if (context) {
        context->Initialize();
    }

    return context;
}