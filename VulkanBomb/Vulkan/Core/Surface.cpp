#include "Surface.h"

expected<void, SurfaceError> Surface::Init(
    VkInstance &instance, GLFWwindow *window, 
    PresentMode presentMode, SurfaceColorFormat format)
{

    if(instance == VK_NULL_HANDLE){
        return unexpected(SurfaceError::SurfaceInstanceError);
    }
    _instance = instance;
    if(glfwCreateWindowSurface(_instance, window, nullptr, &_surface) != VK_SUCCESS){
        return unexpected(SurfaceError::SurfaceInitError);
    }

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_physicalDevice, _surface, &_capabilities);
    vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, _surface, &_formatCount, nullptr);
    _formats.resize(_formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, _surface, &_formatCount, _formats.data());
    vkGetPhysicalDeviceSurfacePresentModesKHR(_physicalDevice, _surface, &_presentModeCount, nullptr);
    _presentModes.resize(_presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(_physicalDevice, _surface, &_presentModeCount, _presentModes.data());
    
    if(surfaceAdjustment(presentMode, format) == false){
        return unexpected(SurfaceError::SurfaceSettingsError);
    }
    return {};
}

bool Surface::surfaceAdjustment(PresentMode presentMode, SurfaceColorFormat surfaceColorFormat)
{
    VkFormat format = static_cast<VkFormat>(surfaceColorFormat);
    VkColorSpaceKHR colorSpace{};

    switch (surfaceColorFormat)
    {
        case SurfaceColorFormat::HDR10:
            colorSpace = VK_COLOR_SPACE_HDR10_ST2084_EXT;
            break;
        case SurfaceColorFormat::HDR:
            colorSpace = VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT;
            break;
        case SurfaceColorFormat::Standard:
            colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
            break;
        case SurfaceColorFormat::Linear:
            colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
            break;
        case SurfaceColorFormat::RGB_Order:
            colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
            break;
        default:
            colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
            break;
    }

    _surfaceFormat = _formats[0];
    for (const auto& availableFormat : _formats) {
        if (availableFormat.format == format &&
            availableFormat.colorSpace == colorSpace) {
            _surfaceFormat = availableFormat;
            break;
        }
    }
    _presentMode = VK_PRESENT_MODE_FIFO_KHR;
    auto targetPresentMode = static_cast<VkPresentModeKHR>(presentMode);
    for (const auto& availableMode : _presentModes) {
        if (availableMode == targetPresentMode) {
            _presentMode = availableMode;
            break;
        }
    }
    return true;
}

// Surface::~Surface()
// {
//     if (_instance != VK_NULL_HANDLE && _surface != VK_NULL_HANDLE) {
//         vkDestroySurfaceKHR(_instance, _surface, nullptr);
//         _surface = VK_NULL_HANDLE;
//     }
// }
