//
// Created by Nutellis on 20-Jan-26.
//

#ifndef ALKYONERENDERENGINE_WINDOW_H
#define ALKYONERENDERENGINE_WINDOW_H

#define RHI_VULKAN_ENABLED 1

#ifdef RHI_D3D12_ENABLED
    #include <windef.h>
#endif

#ifdef RHI_VULKAN_ENABLED
    #include <volk.h>
#endif

#include "GLFW/glfw3.h"



#include "PODTypes.h"
#include <string>
#include <vector>

#include "../rhi/core/DynamicRHI.h"


struct GLFWwindow;
struct GLFWmonitor;
class DynamicRHI;

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

    uint32 width, height;
    size_t frameCount;
    WindowFlags windowFlags;


    bool framebufferResized;

public:
    // rule of 5
    ARWindow();
    ~ARWindow();

public:
    bool Initialize();
    void Terminate() const;
    bool CreateWindow(const std::string& windowName, uint32 inWidth, uint32 inHeight, bool bisFullScreen);
    void ResizeWindow(int width, int height);

    // Callbacks
    static void OnCloseCallback(GLFWwindow* window);
    static void ErrorCallback(int ErrorCode, const char* Description);
    static void OnResizeCallback(GLFWwindow* window, int width, int height);

#ifdef RHI_VULKAN_ENABLED
    std::vector<const char*> GetVulkanRequiredExtensions();
    bool CreateVulkanSurface(const VkInstance & instance, VkSurfaceKHR* surface);
#endif

#ifdef RHI_D3D12_ENABLED
    HWND getWin32Handle() const = 0;
#endif
};



#endif //ALKYONERENDERENGINE_WINDOW_H
