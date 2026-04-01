//
// Created by Nutellis on 20-Jan-26.
//

#include <fstream>
#include <core/Engine.h>
#include <core/ApplicationWindow.h>
#include <GLFW/glfw3.h>

#include "rendering/Mesh.h"
#include <rhi/descriptors/CommandPoolDesc.h>
#include <rhi/descriptors/GraphicsPipelineDesc.h>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "rhi/core/IFrameSync.h"
#include "rhi/core/barriers/ImageBarrier.h"
#include "spdlog/spdlog.h"
#include "utilities/GLTFLoader.h"

#include <utilities/TypeUtilities.h>

AlkyoneRenderEngine::AlkyoneRenderEngine(): window(nullptr), rhi(nullptr), shaderManager(nullptr)
{
}

AlkyoneRenderEngine::~AlkyoneRenderEngine()
{
    delete window;
    delete rhi;
    delete shaderManager;
}

bool AlkyoneRenderEngine::Initialize()
{
    if (glfwInit() == false)
    {
        //LOG(ERROR, "GLFW cannot be initialized");
        //LOG(ERROR, "It's fine, really. Nothing is working though. :) ");

        //LOG(FATAL, "WINDOW MANAGER FAILED INITIATON\n");
        //send terminating signal

        return false;
    }

    spdlog::info("GLFW: Initialized");

    window = new ARWindow();
    window->Initialize();

    //TODO: hardcoded VULKAN for now
    rhi = DynamicRHI::CreateContext(window, RendererBackend::Vulkan);

    shaderManager = new ShaderManager();
    shaderManager->Initialize(rhi->GetSlangTargetOptions());

    return true;
}

void AlkyoneRenderEngine::Terminate()
{
    rhi->Terminate();
    window->Terminate();
    glfwTerminate();
}
void AlkyoneRenderEngine::Run()
{
    //test GLFT loader

    //RenderObject * object = new RenderObject();

    Slang::ComPtr<slang::IModule> slangModule{ shaderManager->slangSession->loadModuleFromSource("triangle", "../shaders/triangle.slang", nullptr, nullptr) };

    GraphicsPipelineDesc desc {};
    desc.shaderModule = slangModule;
    desc.viewportCount = 1;
    desc.scissorCount = 1;
    desc.depthTestEnable = false;
    desc.depthWriteEnable = false;
    desc.depthCompareOp = CompareOp::OP_LESS_EQUAL;
    desc.imageFormats.push_back(FORMAT_B8G8R8A8_SRGB);
    desc.depthImageFormat = FORMAT_D32_SFLOAT_S8_UINT;
    desc.polygonMode = POLYGON_MODE_FILL;
    desc.cullMode = CULL_MODE_NONE;
    desc.frontFace = FRONT_FACE_CLOCKWISE;

    rhi->CreatePipeline(desc);


    MeshGroup* meshgroup = new MeshGroup();

    auto loader = GLTFLoader();

    loader.Load(TODO, TODO);
    loader.PopulateMeshGroup(TODO, *meshgroup);

    while (!glfwWindowShouldClose(window->windowHandle)) {
        glfwPollEvents();

        if (rhi->BeginFrame() == false)
        {
            spdlog::error("Skipping Frame");
            continue;
        }

        rhi->TransitionBarrier(ImageBarrier{
            .sourceStageMask = StageFlagBits::STAGE_COLOUR_ATTACHMENT_OUTPUT,
            .sourceAccessMask = AccessMaskFlagBits::ACCESS_NONE,
            .destStageMask = StageFlagBits::STAGE_COLOUR_ATTACHMENT_OUTPUT,
            .destAccessMask = AccessMaskFlagBits::ACCESS_COLOUR_ATTACHMENT_WRITE,
            .sourceState = STATE_UNDEFINED,
            .destState = STATE_COLOUR_ATTACHMENT,
            .aspectMask = ImageAspectFlagBits::ASPECT_COLOUR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
        });
        //rhi->ClearColour(Vector3f(0.5f,0.5f,0.1f));

        //setup the rendering info

        rhi->BeginRendering();
        //rendering commands here

        //for each mesh in the scene (we only have 1 for now)


        rhi->BindPipeline(0);

        rhi->PrepareVertexBuffer(*meshgroup->meshLodGroups[0]);


        //rhi->SetViewport(0, 0, window->GetWidth(), window->GetHeight());
        //rhi->SetScissor(0, 0, window->GetWidth(), window->GetHeight());

        rhi->Draw();

        rhi->EndRendering();


        rhi->TransitionBarrier(ImageBarrier{
            .sourceStageMask = StageFlagBits::STAGE_ALL_COMMANDS,
            .sourceAccessMask = AccessMaskFlagBits::ACCESS_MEMORY_WRITE,
            .destStageMask = StageFlagBits::STAGE_ALL_COMMANDS,
            .destAccessMask = AccessMaskFlagBits::ACCESS_MEMORY_WRITE | AccessMaskFlagBits::ACCESS_MEMORY_READ,
            .sourceState = STATE_COLOUR_ATTACHMENT,
            .destState = STATE_PRESENT_SRC,
            .aspectMask = ImageAspectFlagBits::ASPECT_COLOUR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
        });
        rhi->EndFrame();
    }
    rhi->WaitIdle();

    meshgroup->Terminate();
}
