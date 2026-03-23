#include "Pipeline.h"

expected<void, PipeLineError> Pipeline::Init(
    VkDevice &device, VkRenderPass &renderPass, 
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
    _shaderStages = shaderStages;

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

    _vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    _vertexInputInfo.vertexBindingDescriptionCount = 0;
    _vertexInputInfo.vertexAttributeDescriptionCount = 0;

    _inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    _inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    _inputAssembly.primitiveRestartEnable = VK_FALSE;

    _viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    _viewportState.viewportCount = 1;
    _viewportState.pViewports = nullptr;
    _viewportState.scissorCount = 1;
    _viewportState.pScissors = nullptr;

    _rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    _rasterizer.depthClampEnable = VK_FALSE;
    _rasterizer.rasterizerDiscardEnable = VK_FALSE;
    _rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    _rasterizer.lineWidth = 1.0f;
    _rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    _rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    _rasterizer.depthBiasEnable = VK_FALSE;

    _multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    _multisampling.sampleShadingEnable = VK_FALSE;
    _multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    _colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    _colorBlendAttachment.blendEnable = VK_FALSE;

    _colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    _colorBlending.logicOpEnable = VK_FALSE;
    _colorBlending.attachmentCount = 1;
    _colorBlending.pAttachments = &_colorBlendAttachment;

    _dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    _dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    _dynamicState.dynamicStateCount = static_cast<uint32_t>(_dynamicStates.size());
    _dynamicState.pDynamicStates = _dynamicStates.data();

    return true;
}

bool Pipeline::createGraphicsPipeline()
{
    VkGraphicsPipelineCreateInfo pipelineCreateInfo{};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.stageCount = 2;
    pipelineCreateInfo.pStages = _shaderStages.data();
    pipelineCreateInfo.pVertexInputState = &_vertexInputInfo;
    pipelineCreateInfo.pInputAssemblyState = &_inputAssembly;
    pipelineCreateInfo.pViewportState = &_viewportState;
    pipelineCreateInfo.pRasterizationState = &_rasterizer;
    pipelineCreateInfo.pMultisampleState = &_multisampling;
    pipelineCreateInfo.pColorBlendState = &_colorBlending;
    pipelineCreateInfo.pDynamicState = &_dynamicState;
    pipelineCreateInfo.layout = _pipelineLayout;
    pipelineCreateInfo.renderPass = _renderPass;
    pipelineCreateInfo.subpass = 0;
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(_device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &_pipeline) != VK_SUCCESS)
    {
        return false;
    }
}

