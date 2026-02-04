//
// Created by Nutellis on 20-Jan-26.
//
#include "core/ApplicationWindow.h"
#include <GLFW/glfw3.h>

#include "spdlog/spdlog.h"

ARWindow::ARWindow() : windowHandle(nullptr), monitorHandle(nullptr), contextHandle(nullptr), width(0), height(0),
                       frameCount(0), windowFlags()
{
    int count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);
    //LOG(INFO, "%d", count);
    monitorHandle = glfwGetPrimaryMonitor();
}

ARWindow::~ARWindow()
{
}

bool ARWindow::Initialize()
{
    //TODO: remove hardcoded. get info from init options json
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);


    //TODO: remove hardcoded. get info from init options json
    const bool success =
        CreateWindow("Alkyone Render Engine", 1280, 720, false)
        && InitializeContext();

    if (success == false)
    {
        spdlog::info("Core Engine: Critical failure during window initialization");
        return success;
    }



    spdlog::info("Core Engine: Window initialized Successfully");
    return success;
}

void ARWindow::Terminate()
{
    glfwDestroyWindow(windowHandle);
}

bool ARWindow::CreateWindow(const std::string& windowName, uint32 inWidth, uint32 inHeight, bool bisFullScreen)
{
    name = windowName;
    width = inWidth;
    height = inHeight;

    if (bisFullScreen)
    {
        windowHandle = glfwCreateWindow(width, height, windowName.c_str(), monitorHandle, nullptr);
    } else
    {
        windowHandle = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    }

    if (windowHandle == nullptr)
    {
        spdlog::error("Failed to initialize GLFW Window");
        return false;
    }
        // log window initialized
    windowFlags.bIsFullScreen = bisFullScreen;
    windowFlags.bIsActive = true;

    glfwSetWindowSizeLimits(windowHandle, 720, 480,
glfwGetVideoMode(monitorHandle)->width,
glfwGetVideoMode(monitorHandle)->height);


    glfwSetWindowAspectRatio(windowHandle, 16, 9);

    // Make the window's context current
    glfwMakeContextCurrent(windowHandle);
    glfwSetWindowUserPointer(windowHandle, this);

    glfwSetWindowCloseCallback(windowHandle, OnCloseCallback);

    //Set GLFW error callback
    //	glfwSetErrorCallback(ErrorCallback);

    glfwSetFramebufferSizeCallback(windowHandle, OnResizeCallback);
    glfwSetWindowSizeCallback(windowHandle, OnResizeCallback);

    spdlog::info("Created GLFW window {}", name);
    return true;
}

bool ARWindow::InitializeContext()
{
    //TODO: remove hardcoded. get info from init options json
    contextHandle = IGraphicsContext::CreateContext(windowHandle, RendererBackend::Vulkan);

    if (contextHandle == nullptr)
    {
        spdlog::error("Context creation failed");
        return false;
    }

    return true;
}


void ARWindow::ResizeWindow(int width, int height)
{
    //LOG(DEBUG, "Resizing Window");
    //	glfwSetWindowSize(Window, Width, Height);
}

void ARWindow::OnCloseCallback(GLFWwindow * window)
{
   // LOG(INFO, "User attempted to close the window!!");
}

//Error
void ARWindow::ErrorCallback(int errorCode, const char * description)
{
    //LOG(ERROR, "%s", Description);
}

void ARWindow::OnResizeCallback(GLFWwindow * window, int width, int height)
{
   // int newWidth, newHeight;
   // glfwGetFramebufferSize(window, &newWidth, &newHeight);
   // glViewport(0, 0, newWidth, newHeight);

    //LOG(CMD, "Window is resizing. Width: %d Height: %d", width, height);
}
