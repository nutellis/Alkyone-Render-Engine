//
// Created by Nutellis on 20-Jan-26.
//

#ifndef ALKYONERENDERENGINE_WINDOW_H
#define ALKYONERENDERENGINE_WINDOW_H

#include "PODTypes.h"
#include <string>

#include "context/IGraphicsContext.h"
#include "GLFW/glfw3.h"

struct GLFWwindow;
struct GLFWmonitor;
class IGraphicsContext;

struct WindowFlags
{
    bool bIsResizable : 1;
    bool bIsActive : 1;
    bool bIsFullScreen : 1;
    bool bIsMinimized : 1;
    bool bIsMaximized : 1;
    bool bIsRestored : 1;
    bool bIsVisible : 1;
    bool bVSyncEnabled : 1;
};


class ARWindow
{
public:
    std::string name;
    GLFWwindow * windowHandle;
    GLFWmonitor * monitorHandle;

    IGraphicsContext* contextHandle;

    uint32 width, height;
    size_t frameCount;
    WindowFlags windowFlags;


public:
    // rule of 5
    ARWindow();
    ~ARWindow();

public:
    bool Initialize();
    void Terminate();
    bool CreateWindow(const std::string& windowName, uint32 inWidth, uint32 inHeight, bool bisFullScreen);
    bool InitializeContext();
    void ResizeWindow(int width, int height);

    // Callbacks
    static void OnCloseCallback(GLFWwindow* window);
    static void ErrorCallback(int ErrorCode, const char* Description);
    static void OnResizeCallback(GLFWwindow* window, int width, int height);
};



#endif //ALKYONERENDERENGINE_WINDOW_H
