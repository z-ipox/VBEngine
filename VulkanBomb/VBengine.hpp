#ifndef VBENGINE_HPP
#define VBENGINE_HPP

#include <string>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>


class VBengine {
private:
    std::string _programName;
    int _width, _height;

    VkInstance _instance;
    VkSurfaceKHR _surface;
    VkDevice _device;
    VkSwapchainKHR _swapchain;

    GLFWwindow* _window;

public:
    VBengine(const std::string& programName, int width, int height) : _programName(programName)
    , _width(width)
    , _height(height)
    , _window(nullptr)
    , _surface(VK_NULL_HANDLE)
    , _device(VK_NULL_HANDLE)
    , _swapchain(VK_NULL_HANDLE);
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

#endif