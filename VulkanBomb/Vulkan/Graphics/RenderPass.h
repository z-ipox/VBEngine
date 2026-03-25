#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <expected>

enum class RenderPassErrors{
    RederPassCreateRenderPassError,
    RenderPassDeviceError,
    RenderPassSurfaceError,
};

using namespace std;

class RenderPass{
    private:
        VkAttachmentDescription _colorAttachment;
        VkAttachmentReference _colorAttachmentRef;
        VkSubpassDescription _subpass;
        VkSubpassDependency _dependency;
        VkRenderPass _renderPass;
        VkDevice _device;
        VkSurfaceFormatKHR _surfaceFormat;

        bool CreateRenderPass();
    public:
        RenderPass(){}
        ~RenderPass(){};

        expected<void, RenderPassErrors> Init(VkSurfaceFormatKHR &surfaceFormat, VkDevice device);

        VkRenderPass getRenderPass() const { return _renderPass; }

};