//
// Created by Nutellis on 20-Jan-26.
//

#include <core/Engine.h>
#include <core/ApplicationWindow.h>
#include <GLFW/glfw3.h>

#include "rendering/Mesh.h"
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

    MeshGroup* meshgroup = new MeshGroup();

    auto loader = GLTFLoader();

    loader.Load();
    loader.PopulateMeshGroup(*meshgroup);

    while (!glfwWindowShouldClose(window->windowHandle)) {
        glfwPollEvents();
    }
}
