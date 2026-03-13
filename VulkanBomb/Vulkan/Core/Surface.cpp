#include "Surface.h"

expected<void, SurfaceError> Surface::Init(
    VkInstance instance, GLFWwindow *window){

    if(instance == nullptr || instance == VK_NULL_HANDLE){
        return unexpected(SurfaceError::SurfaceInstanceError);
    }
    if(glfwCreateWindowSurface(instance, window, nullptr, &_surface) != VK_SUCCESS){
        return unexpected(SurfaceError::SurfaceInitError);
    }

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_physicalDevice, _surface, &_capabilities);
    vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, _surface, &_formatCount, nullptr);
    _formats.resize(_formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, _surface, &_formatCount, _formats.data());
    vkGetPhysicalDeviceSurfacePresentModesKHR(_physicalDevice, _surface, &_presentModeCount, nullptr);
    _presentModes.resize(_presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(_physicalDevice, _surface, &_presentModeCount, _presentModes.data());
    
    if(surfaceAdjustment() == false){
        return unexpected(SurfaceError::SurfaceSettingsError);
    }
}

bool Surface::surfaceAdjustment(){
    _surfaceFormat = _formats[0];
    for (const auto& availableFormat : _formats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            _surfaceFormat = availableFormat;
            break;
        }
    }

    _presentMode = VK_PRESENT_MODE_FIFO_KHR;
    for (const auto& availableMode : _presentModes) {
        if (availableMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            _presentMode = availableMode;
            break;
        }
    }
    return true;
}

