#pragma once
#include <vector>
#include <expected>
#include <vulkan/vulkan.h>

using namespace std;

enum class FrameBufferError{
    CreateFrameBufferError,
    FrameBufferImageCountError,
    FrameBufferRenderPassError,
    FrameBufferSwapchainImageViewsError,
    FrameBufferDeviceError,
    FrameBufferExtent2DError
};

class FrameBuffer{
    private:
        vector<VkFramebuffer> _framebuffers;
    public:
        FrameBuffer(){};
        ~FrameBuffer(){};

    expected<void, FrameBufferError> Init(
        uint32_t imageCount, vector<VkImageView> &swapchainImageViews,
        VkRenderPass renderPass, VkDevice device, VkExtent2D &extent2D);

    const vector<VkFramebuffer> getFrameBuffers() const { return _framebuffers; }

};