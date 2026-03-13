#pragma once
#include <expected>
#include <string>
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <vector>

using namespace std;

enum class SurfaceError{
    SurfaceInstanceError,
    SurfaceInitError,
    SurfaceSettingsError,
};

class Surface{

    private:
        VkSurfaceKHR _surface;
        VkSurfaceCapabilitiesKHR _capabilities;
        VkPhysicalDevice _physicalDevice; 
        uint32_t _formatCount,
                 _presentModeCount;
        vector<VkSurfaceFormatKHR> _formats;
        vector<VkPresentModeKHR> _presentModes;
        VkPresentModeKHR _presentMode;
        VkSurfaceFormatKHR _surfaceFormat;

        bool surfaceAdjustment();

    public:
        Surface() : _surface(VK_NULL_HANDLE)
        , _physicalDevice(VK_NULL_HANDLE) {}
        ~Surface();

        expected<void, SurfaceError> Init(
            VkInstance instance, GLFWwindow *window);
        
        VkSurfaceKHR getSurface() const { return _surface; }
        VkSurfaceCapabilitiesKHR getCapabilities() const { return _capabilities; }
        uint32_t getFormatCount() const { return _formatCount; }
        uint32_t getPresentModeCount() const { return _presentModeCount; }
        VkSurfaceFormatKHR getSurfaceFormat() const { return _surfaceFormat; }
        VkPresentModeKHR getPresentMode() const { return _presentMode; }

};