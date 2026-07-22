//
// Created by Nutellis on 20-Jan-26.
//

#include <fstream>
#include <core/Engine.h>
#include <core/ApplicationWindow.h>
#include <GLFW/glfw3.h>

#include <rhi/descriptors/GraphicsPipelineDesc.h>

#include "rhi/core/barriers/ImageBarrier.h"
#include "spdlog/spdlog.h"
#include "resources/GLTFImporter.h"

#include <utilities/TypeUtilities.h>

#include "resources/ResourceManager.h"
#include "rhi/descriptors/ShaderCompileDesc.h"

AlkyoneRenderEngine::AlkyoneRenderEngine():
    window(nullptr),
    rhi(nullptr),
    shaderManager(nullptr),
    resourceManager(nullptr),
    sceneManager(nullptr)
{
}

AlkyoneRenderEngine::~AlkyoneRenderEngine()
{
    delete sceneManager;
    delete resourceManager;
    delete shaderManager;
    delete rhi;
    delete window;
}

bool AlkyoneRenderEngine::Initialize()
{
    if (glfwInit() == false)
    {
        spdlog::error("GLFW cannot be initialized");
        spdlog::error( "It's fine, really. Nothing is working though. :) ");
        spdlog::error( "Fatal Error - Alkyone Render Engine \n");

        return false;
    }

    spdlog::info("GLFW: Initialized");

    window = new ARWindow();
    window->Initialize();

    //TODO: hardcoded VULKAN for now
    rhi = DynamicRHI::CreateContext(window, RendererBackend::Vulkan);

    sceneManager = new SceneManager();
    sceneManager->Initialize();

    resourceManager = new ResourceManager(*rhi);
    resourceManager->Initialize();

    return true;
}

void AlkyoneRenderEngine::Terminate() const
{
    sceneManager->Terminate();

    resourceManager->Terminate();
    //shaderManager->Terminate();
    rhi->Terminate();
    window->Terminate();
    glfwTerminate();
}
void AlkyoneRenderEngine::Run() const
{
    std::string filename = std::string("../assets/chess/chess_set_1k.gltf");
    ParsedData sceneData = GLTFImporter::ImportSceneFile(filename);

    resourceManager->LoadSceneGeometry(sceneData);
    sceneManager->SetupSceneHierarchy(sceneData);

    // load a simple shader for testing
    ShaderHandle sh = resourceManager->LoadShader("basicShader.slang", "vertexMain", "fragmentMain");

    GraphicsPipelineDesc desc {};
    desc.shader = sh,
    desc.viewportCount = 1;
    desc.scissorCount = 1;
    desc.depthTestEnable = false;
    desc.depthWriteEnable = false;
    desc.depthCompareOp = OP_LESS_EQUAL;
    desc.imageFormats.push_back(FORMAT_B8G8R8A8_SRGB);
    desc.depthImageFormat = FORMAT_D32_SFLOAT_S8_UINT;
    desc.polygonMode = POLYGON_MODE_FILL;
    desc.cullMode = CULL_MODE_NONE;
    desc.frontFace = FRONT_FACE_CLOCKWISE;

    rhi->CreatePipeline(desc);


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

        rhi->BindPipeline(0);

        //rhi->PrepareVertexBuffer(*meshgroup);


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
}
