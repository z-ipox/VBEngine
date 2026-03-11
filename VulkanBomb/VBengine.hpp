#ifndef VBENGINE_HPP
#define VBENGINE_HPP

#include <string>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

class VBengine {
private:
    std::string _programName;
    int _width, _height;

    GLFWwindow* _window;

public:
    VBengine(const std::string& programName, int width, int height);
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