#ifndef VBENGINE_HPP
#define VBENGINE_HPP

#include <string>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

class VBengine {
private:
    static std::string _programName;
    static int _width, _height;

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
    static int GetWidth() { return _width; }
    static int GetHeight() { return _height; }
};

#endif