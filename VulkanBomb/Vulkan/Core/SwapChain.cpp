
#include "SwapChain.h"

expected<void, SwapChainError>SwapChain::Init(
            VkDevice &device, VkSurfaceKHR &surface, 
            VkSurfaceFormatKHR &surfaceFormat, VkPresentModeKHR &presentMode, 
            VkSurfaceCapabilitiesKHR &capabilities, uint32_t &imageCount, 
            int &graphicsQueueFamilyIndex, int &presentQueueFamilyIndex)
{

    if (device == VK_NULL_HANDLE){
        unexpected(SwapChainError::SwapChainDeviceError);
    }
    if (capabilities.minImageCount == 0 ){
        unexpected(SwapChainError::SwapChainCapabilitiesError);
    }
    if (imageCount == 0){
        unexpected(SwapChainError::SwapChainImageCountError);
    }
    if (surfaceFormat.format == VK_FORMAT_UNDEFINED){
        unexpected(SwapChainError::SwapChainSurfaceFormatError);
    }

    _device = device;
    _extent2D = capabilities.currentExtent;
    _imageCount = imageCount;
    _surfaceFormat = surfaceFormat;

    if (createSwapChain(surface, capabilities, presentMode) == false) {
        return unexpected(SwapChainError::SwapChainInitError);
    }
    if (getImagesSwapChain() == false){
        return unexpected(SwapChainError::SwapChainCreateImageViewsError);
    }

}

bool SwapChain::createSwapChain(
    VkSurfaceKHR &surface, VkSurfaceCapabilitiesKHR &capabilities,
    VkPresentModeKHR &presentMode)
{
    _swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    _swapchainCreateInfo.surface = surface;
    _swapchainCreateInfo.minImageCount = _imageCount;
    _swapchainCreateInfo.imageFormat = _surfaceFormat.format;
    _swapchainCreateInfo.imageColorSpace = _surfaceFormat.colorSpace;
    _swapchainCreateInfo.imageExtent = _extent2D;
    _swapchainCreateInfo.imageArrayLayers = 1;
    _swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // VK_IMAGE_USAGE_TRANSFER_DST_BIT  VK_IMAGE_USAGE_SAMPLED_BIT;
    _swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    _swapchainCreateInfo.preTransform = capabilities.currentTransform;
    _swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    _swapchainCreateInfo.presentMode = presentMode;
    _swapchainCreateInfo.clipped = VK_TRUE;
    _swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

    if (_graphicsQueueFamilyIndex != _presentQueueFamilyIndex) {
        uint32_t queueFamilyIndices[] = {
            static_cast<uint32_t>(_graphicsQueueFamilyIndex),
            static_cast<uint32_t>(_presentQueueFamilyIndex)
        };
        _swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        _swapchainCreateInfo.queueFamilyIndexCount = 2;
        _swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }

    if (vkCreateSwapchainKHR(_device, &_swapchainCreateInfo, nullptr, &_swapchain) != VK_SUCCESS){
        return false;
    }
    return true;
}

bool SwapChain::getImagesSwapChain()
{
    vkGetSwapchainImagesKHR(_device, _swapchain, &_imageCount, nullptr);
    _swapchainImages.resize(_imageCount);
    vkGetSwapchainImagesKHR(_device, _swapchain, &_imageCount, _swapchainImages.data());
    _swapchainImageViews.resize(_imageCount);

    for (size_t i = 0; i < _imageCount; i++) {
        VkImageViewCreateInfo imageViewCreateInfo = {};
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.image = _swapchainImages[i];
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = _surfaceFormat.format;
        imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(_device, &imageViewCreateInfo, nullptr, &_swapchainImageViews[i]) != VK_SUCCESS) {
            return false;
        }
    }
    return true;
}
