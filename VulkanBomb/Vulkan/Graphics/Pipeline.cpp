#include "Pipeline.h"

expected<void, PipeLineError> Pipeline::Init(
    VkDevice device, VkRenderPass renderPass, 
    const vector<VkPipelineShaderStageCreateInfo> &shaderStages)
{
    if (device == VK_NULL_HANDLE){
        return unexpected(PipeLineError::PipeLineDeviceError);
    }
    if (renderPass == VK_NULL_HANDLE){
        return unexpected(PipeLineError::PipeLineRederPassError);
    }
    if (shaderStages.empty()){
        return unexpected(PipeLineError::PipeLineNoShaderStagesError);
    }

    _device = device;
    _renderPass = renderPass;
    _shaderStageCreateInfos = shaderStages;

    if (createPipelineLayout() == false){
        return unexpected(PipeLineError::PipeLineCreateLayoutError);
    }
    if (createGraphicsPipeline() == false){
        return unexpected(PipeLineError::PipeLineCreateGraphicsError);
    }
    return {};
}

bool Pipeline::createPipelineLayout()
{
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.setLayoutCount = 0;
    pipelineLayoutCreateInfo.pushConstantRangeCount = 0;

    if (vkCreatePipelineLayout(_device, &pipelineLayoutCreateInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
    {
        return false;
    }

    _vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    _vertexInputStateCreateInfo.vertexBindingDescriptionCount = 0;
    _vertexInputStateCreateInfo.vertexAttributeDescriptionCount = 0;

    _inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    _inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    _inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

    _viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    _viewportStateCreateInfo.viewportCount = 1;
    _viewportStateCreateInfo.pViewports = nullptr;
    _viewportStateCreateInfo.scissorCount = 1;
    _viewportStateCreateInfo.pScissors = nullptr;

    _rasterizerStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    _rasterizerStateCreateInfo.depthClampEnable = VK_FALSE;
    _rasterizerStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
    _rasterizerStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
    _rasterizerStateCreateInfo.lineWidth = 1.0f;
    _rasterizerStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    _rasterizerStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    _rasterizerStateCreateInfo.depthBiasEnable = VK_FALSE;

    _multisamplingStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    _multisamplingStateCreateInfo.sampleShadingEnable = VK_FALSE;
    _multisamplingStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    _colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    _colorBlendAttachmentState.blendEnable = VK_FALSE;

    _colorBlendingStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    _colorBlendingStateCreateInfo.logicOpEnable = VK_FALSE;
    _colorBlendingStateCreateInfo.attachmentCount = 1;
    _colorBlendingStateCreateInfo.pAttachments = &_colorBlendAttachmentState;

    _dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    _dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    _dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(_dynamicStates.size());
    _dynamicStateCreateInfo.pDynamicStates = _dynamicStates.data();

    return true;
}

bool Pipeline::createGraphicsPipeline()
{
    VkGraphicsPipelineCreateInfo pipelineCreateInfo{};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.stageCount = 2;
    pipelineCreateInfo.pStages = _shaderStageCreateInfos.data();
    pipelineCreateInfo.pVertexInputState = &_vertexInputStateCreateInfo;
    pipelineCreateInfo.pInputAssemblyState = &_inputAssemblyStateCreateInfo;
    pipelineCreateInfo.pViewportState = &_viewportStateCreateInfo;
    pipelineCreateInfo.pRasterizationState = &_rasterizerStateCreateInfo;
    pipelineCreateInfo.pMultisampleState = &_multisamplingStateCreateInfo;
    pipelineCreateInfo.pColorBlendState = &_colorBlendingStateCreateInfo;
    pipelineCreateInfo.pDynamicState = &_dynamicStateCreateInfo;
    pipelineCreateInfo.layout = _pipelineLayout;
    pipelineCreateInfo.renderPass = _renderPass;
    pipelineCreateInfo.subpass = 0;
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(_device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &_pipeline) != VK_SUCCESS)
    {
        return false;
    }
}

