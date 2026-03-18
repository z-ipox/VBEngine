#include "RenderPass.h"


expected<void, RenderPassErrors> RenderPass::Init(
    VkSurfaceFormatKHR &surfaceFormat, VkDevice &device)
{
    if (device == VK_NULL_HANDLE){
        return unexpected(RenderPassErrors::RenderPassDeviceError);
    }
    
    _surfaceFormat = surfaceFormat;
    _device = device;

    if (CreateRenderPass() == false){
        return unexpected(RenderPassErrors::RederPassCreateRenderPassError);
    }
}

bool RenderPass::CreateRenderPass(){
    _colorAttachment.format = _surfaceFormat.format;
    _colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    _colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    _colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    _colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    _colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    _colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    _colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    _colorAttachmentRef.attachment = 0;
    _colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    _subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    _subpass.colorAttachmentCount = 1;
    _subpass.pColorAttachments = &_colorAttachmentRef;

    _dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    _dependency.dstSubpass = 0;
    _dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    _dependency.srcAccessMask = 0;
    _dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    _dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    _renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    _renderPassCreateInfo.attachmentCount = 1;
    _renderPassCreateInfo.pAttachments = &_colorAttachment;
    _renderPassCreateInfo.subpassCount = 1;
    _renderPassCreateInfo.pSubpasses = &_subpass;
    _renderPassCreateInfo.dependencyCount = 1;
    _renderPassCreateInfo.pDependencies = &_dependency;

    if (vkCreateRenderPass(_device, &_renderPassCreateInfo, nullptr, &_renderPass) != VK_SUCCESS) {
        return false;
    }
    return true;
}