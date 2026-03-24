
#include "SwapChain.h"

expected<void, SwapChainError> SwapChain::Init(
            VkDevice device, VkSurfaceKHR surface, 
            VkSurfaceFormatKHR &surfaceFormat, VkPresentModeKHR &presentMode, 
            VkSurfaceCapabilitiesKHR &capabilities,
            uint32_t graphicsQueueFamilyIndex, uint32_t presentQueueFamilyIndex)
{

    if (device == VK_NULL_HANDLE){
        return unexpected(SwapChainError::SwapChainInvalidDevice);
    }
    if (surface == VK_NULL_HANDLE){
        return unexpected(SwapChainError::SwapChainInvalidSurface);
    }
    if (presentMode < 0){
        return unexpected(SwapChainError::SwapChainInvalidPresentMode);
    }
    if (capabilities.minImageCount == 0 ){
        return unexpected(SwapChainError::SwapChainCapabilitiesError);
    }
    if (surfaceFormat.format == VK_FORMAT_UNDEFINED){
        return unexpected(SwapChainError::SwapChainSurfaceFormatError);
    }

    _device = device;
    _surface = surface;
    _presentMode = presentMode;
    _capabilities = capabilities;
    _extent2D = capabilities.currentExtent;
    _imageCount = capabilities.minImageCount + 1;
    _surfaceFormat = surfaceFormat;

    if (createSwapChain() == false) {
        return unexpected(SwapChainError::SwapChainInitError);
    }
    if (getImagesSwapChain() == false){
        return unexpected(SwapChainError::SwapChainCreateImageViewsError);
    }
    return {};
}

bool SwapChain::createSwapChain()
{
    VkSwapchainCreateInfoKHR swapchainCreateInfo{};
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.surface = _surface;
    swapchainCreateInfo.minImageCount = _imageCount;
    swapchainCreateInfo.imageFormat = _surfaceFormat.format;
    swapchainCreateInfo.imageColorSpace = _surfaceFormat.colorSpace;
    swapchainCreateInfo.imageExtent = _extent2D;
    swapchainCreateInfo.imageArrayLayers = 1;
    swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // VK_IMAGE_USAGE_TRANSFER_DST_BIT  VK_IMAGE_USAGE_SAMPLED_BIT;
    swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchainCreateInfo.preTransform = _capabilities.currentTransform;
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCreateInfo.presentMode = _presentMode;
    swapchainCreateInfo.clipped = VK_TRUE;
    swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

    if (_graphicsQueueFamilyIndex != _presentQueueFamilyIndex) {
        uint32_t queueFamilyIndices[] = {
            static_cast<uint32_t>(_graphicsQueueFamilyIndex),
            static_cast<uint32_t>(_presentQueueFamilyIndex)
        };
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchainCreateInfo.queueFamilyIndexCount = 2;
        swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }

    if (vkCreateSwapchainKHR(_device, &swapchainCreateInfo, nullptr, &_swapchain) != VK_SUCCESS){
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

// SwapChain::~SwapChain(){

// }