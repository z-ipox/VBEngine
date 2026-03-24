#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <expected>
#include <set>

using namespace std;

enum class SwapChainError{
    SwapChainInitError,
    SwapChainCreateImageViewsError,
    SwapChainSurfaceFormatError,
    SwapChainInvalidDevice,
    SwapChainCapabilitiesError,
    SwapChainImageCountError,
    SwapChainInvalidSurface,
    SwapChainInvalidPresentMode
};

class SwapChain{
    private:
        VkExtent2D _extent2D;
        VkSwapchainKHR _swapchain;
        VkDevice _device;
        VkSurfaceKHR _surface; 
        VkSurfaceCapabilitiesKHR _capabilities;
        VkPresentModeKHR _presentMode;
        vector<VkImage> _swapchainImages;
        vector<VkImageView> _swapchainImageViews;
        VkSurfaceFormatKHR _surfaceFormat;
        uint32_t _imageCount,
            _graphicsQueueFamilyIndex,
            _presentQueueFamilyIndex;

        bool createSwapChain();
        bool getImagesSwapChain();

    public:
        SwapChain() : 
            _swapchain(VK_NULL_HANDLE)
            , _device(VK_NULL_HANDLE){}

        ~SwapChain();

        expected<void, SwapChainError> Init(
            VkDevice device, VkSurfaceKHR surface, 
            VkSurfaceFormatKHR &surfaceFormat, VkPresentModeKHR &presentMode, 
            VkSurfaceCapabilitiesKHR &capabilities,
            uint32_t graphicsQueueFamilyIndex, uint32_t presentQueueFamilyIndex);

        VkSwapchainKHR getSwapchain() const { return _swapchain; }
        VkExtent2D getExtent2D() const { return _extent2D; }
        vector<VkImage> getSwapChainImages() const { return _swapchainImages; }
        vector<VkImageView> getSwapChainImageViews() const { return _swapchainImageViews; }
};