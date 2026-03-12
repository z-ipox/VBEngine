#pragma once
#include <expected>
#include <string>
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <vector>

enum class SurfaceError{
    SurfaceInstanceError,
    SurfaceInitError,
};

class Surface{

    private:
        VkSurfaceKHR _surface;
        VkSurfaceCapabilitiesKHR _capabilities;
        VkPhysicalDevice _physicalDevice; 
        uint32_t _formatCount;
        uint32_t _presentModeCount;
        vector<VkSurfaceFormatKHR> _formats;
        vector<VkPresentModeKHR> _presentModes;
        VkPresentModeKHR _presentMode;
        VkSurfaceFormatKHR _surfaceFormat;

        bool getInfoSurface();

    public:
        Surface() : _surface(VK_NULL_HANDLE)
        , _physicalDevice(VK_NULL_HANDLE) {}
        ~Surface(){}

        std::expected<void, SurfaceError> Init(
            VkInstance instance, GLFWwindow *window, VkPhysicalDevice &physicalDevice);
        
        VkSurfaceKHR getSurface() const { return _surface; }
        VkSurfaceCapabilitiesKHR getCapabilities() const { return _capabilities; }

};