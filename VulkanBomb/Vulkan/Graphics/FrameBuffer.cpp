#include "FrameBuffer.h"

expected<void, FrameBufferError> FrameBuffer::Init(
    uint32_t imageCount, vector<VkImageView> &swapchainImageViews,
    VkRenderPass renderPass, VkDevice device, VkExtent2D &extent2D)
{
    
    if (imageCount < 1){
        return unexpected(FrameBufferError::FrameBufferImageCountError);
    }
    if (swapchainImageViews.size() < 1){
        return unexpected(FrameBufferError::FrameBufferSwapchainImageViewsError);
    }
    if (renderPass == VK_NULL_HANDLE){
        return unexpected(FrameBufferError::FrameBufferRenderPassError);
    }
    if (device == VK_NULL_HANDLE){
        return unexpected(FrameBufferError::FrameBufferDeviceError);
    }  
    if (extent2D.width == 0 || extent2D.height == 0){
        return unexpected(FrameBufferError::FrameBufferExtent2DError);
    }

    _framebuffers.resize(imageCount);
    for (size_t i = 0; i < imageCount; i++) {
        VkImageView attachments[] = { swapchainImageViews[i] };

        VkFramebufferCreateInfo framebufferCreateInfo = {};
        framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfo.renderPass = renderPass;
        framebufferCreateInfo.attachmentCount = 1;
        framebufferCreateInfo.pAttachments = attachments;
        framebufferCreateInfo.width = extent2D.width;
        framebufferCreateInfo.height = extent2D.height;
        framebufferCreateInfo.layers = 1;

        if (vkCreateFramebuffer(device, &framebufferCreateInfo, nullptr, &_framebuffers[i]) != VK_SUCCESS) {
            return unexpected(FrameBufferError::CreateFrameBufferError);
        }
    }
    return {};
}