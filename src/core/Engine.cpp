//
// Created by Nutellis on 20-Jan-26.
//

#include <fstream>
#include <core/Engine.h>
#include <core/ApplicationWindow.h>
#include <GLFW/glfw3.h>

#include "rendering/Mesh.h"
#include "rhi/GraphicsPipelineDesc.h"
#include "spdlog/spdlog.h"
#include "utilities/GLTFLoader.h"

AlkyoneRenderEngine::AlkyoneRenderEngine() : window(nullptr)
{
}

AlkyoneRenderEngine::~AlkyoneRenderEngine()
{
    delete window;
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

    shaderManager = new ShaderManager();

    //TODO: a getter would be nice
    shaderManager->Initialize(window->contextHandle->slangTargetOptions);

    return true;
}

void AlkyoneRenderEngine::Terminate()
{
    window->Terminate();
    glfwTerminate();
}

void AlkyoneRenderEngine::Run()
{
    //test GLFT loader

    //RenderObject * object = new RenderObject();

    Slang::ComPtr<slang::IModule> slangModule{ shaderManager->slangSession->loadModuleFromSource("triangle", "../shaders/triangle.slang", nullptr, nullptr) };
    Slang::ComPtr<ISlangBlob> spirv;
    slangModule->getTargetCode(0, spirv.writeRef());

    //create a pipeline
    // Slang::ComPtr<slang::IBlob> diagnostics;
    //
    // Slang::ComPtr<slang::IModule> slangModule{
    //     shaderManager->slangSession->loadModule("triangle", diagnostics.writeRef())
    // };
    //
    // if (!slangModule)
    // {
    //     if (diagnostics)
    //         std::cerr << (const char*)diagnostics->getBufferPointer() << std::endl;
    // }
    GraphicsPipelineDesc desc;
    desc.vertexShaderModule = slangModule;
    desc.fragmentShaderModule = slangModule;
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

    window->contextHandle->CreateGraphicsPipeline(desc);


    MeshGroup* meshgroup = new MeshGroup();

    auto loader = GLTFLoader();

    loader.Load();
    loader.PopulateMeshGroup(*meshgroup);

    while (!glfwWindowShouldClose(window->windowHandle)) {
        glfwPollEvents();
    }
}
