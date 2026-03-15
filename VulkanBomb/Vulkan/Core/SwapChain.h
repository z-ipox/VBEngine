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
    SwapChainDeviceError,
    SwapChainCapabilitiesError,
    SwapChainImageCountError,
};

class SwapChain{
    private:
        VkExtent2D _extent2D;
        VkSwapchainKHR _swapchain;
        VkSwapchainCreateInfoKHR _swapchainCreateInfo;
        VkDevice _device;
        vector<VkImage> _swapchainImages;
        vector<VkImageView> _swapchainImageViews;
        uint32_t _imageCount;
        VkSurfaceFormatKHR _surfaceFormat;
        int _graphicsQueueFamilyIndex,
            _presentQueueFamilyIndex;

        bool createSwapChain(
            VkSurfaceKHR &surface, VkSurfaceCapabilitiesKHR &capabilities,
            VkPresentModeKHR &presentMode);

        bool getImagesSwapChain();

    public:
        SwapChain() : 
            _swapchain(VK_NULL_HANDLE)
            , _device(VK_NULL_HANDLE){}

        ~SwapChain();

        expected<void, SwapChainError> Init(
            VkDevice &device, VkSurfaceKHR &surface, 
            VkSurfaceFormatKHR &surfaceFormat, VkPresentModeKHR &presentMode, 
            VkSurfaceCapabilitiesKHR &capabilities, uint32_t &imageCount, 
            int &graphicsQueueFamilyIndex, int &presentQueueFamilyIndex);

        VkSwapchainKHR getSwapchain() const { return _swapchain; }
        VkExtent2D getExtent2D() const { return _extent2D; }
        vector<VkImage> getSwapChainImages() const { return _swapchainImages; }
        vector<VkImageView> getSwapChainImageViews() const { return _swapchainImageViews; }
};