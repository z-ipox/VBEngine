#pragma once
#include <string>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <ostream>
#include "Vulkan/Core/Instance.h"
#include "Vulkan/Core/Device.h"
#include "Vulkan/Core/Surface.h"

enum class PresentMode {
    VSync,                  // VK_PRESENT_MODE_FIFO_KHR
    VSyncAdaptive,          // VK_PRESENT_MODE_FIFO_RELAXED_KHR
    DoubleBuffered,         // VK_PRESENT_MODE_FIFO_KHR
    TripleBuffered,         // VK_PRESENT_MODE_MAILBOX_KHR
    Unlimited               // VK_PRESENT_MODE_IMMEDIATE_KHR
};

enum class SurfaceColorFormat {
    Standard,               // VK_FORMAT_B8G8R8A8_SRGB
    Linear,                 // VK_FORMAT_B8G8R8A8_UNORM
    HDR,                    // VK_FORMAT_R16G16B16A16_SFLOAT
    HDR10,                  // VK_FORMAT_A2B10G10R10_UNORM_PACK32
    RGB_Order,              // VK_FORMAT_R8G8B8A8_SRGB
};

class VBengine {
    private:
        std::string _programName;
        int _width, _height;
        VkInstance _instance;
        VkSurfaceKHR _surface;
        VkDevice _device;
        VkPhysicalDevice _physicalDevice;
        VkSwapchainKHR _swapchain;
        VkSurfaceCapabilitiesKHR _capabilities;
        GLFWwindow* _window;

        Device _vulkanBombDevice;
        Instance _vulkanBombInstance;
        Surface _vulkanBombSurface;

    public:
        VBengine(const std::string& programName, int width, int height) : _programName(programName)
        , _width(width)
        , _height(height)
        , _window(nullptr)
        , _surface(VK_NULL_HANDLE)
        , _device(VK_NULL_HANDLE)
        , _swapchain(VK_NULL_HANDLE) {}
        ~VBengine();

        VBengine(const VBengine&) = delete;
        VBengine& operator=(const VBengine&) = delete;

        void Init();
        void Run();
        void Cleanup();

        GLFWwindow* GetWindow() const { return _window; }
        int GetWidth() { return _width; }
        int GetHeight() { return _height; }
};
