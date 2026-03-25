#pragma once
#include <expected>
#include <vulkan/vulkan.h>
#include <vector>

using namespace std;

enum class PipeLineError{
    PipeLineDeviceError,
    PipeLineCreateLayoutError,
    PipeLineCreateGraphicsError,
    PipeLineRederPassError,
    PipeLineNoShaderStagesError
};

class Pipeline{
    private:
        VkDevice _device;
        vector<VkPipelineShaderStageCreateInfo> _shaderStageCreateInfos;
        VkRenderPass _renderPass;
        VkPipeline _pipeline;
        VkPipelineLayout _pipelineLayout;
        VkPipelineVertexInputStateCreateInfo _vertexInputStateCreateInfo{};
        VkPipelineInputAssemblyStateCreateInfo _inputAssemblyStateCreateInfo{};
        VkPipelineViewportStateCreateInfo _viewportStateCreateInfo{};
        VkPipelineRasterizationStateCreateInfo _rasterizerStateCreateInfo{};
        VkPipelineMultisampleStateCreateInfo _multisamplingStateCreateInfo{};
        VkPipelineColorBlendAttachmentState _colorBlendAttachmentState{};
        VkPipelineColorBlendStateCreateInfo _colorBlendingStateCreateInfo{};
        VkPipelineDynamicStateCreateInfo _dynamicStateCreateInfo{};
        vector<VkDynamicState> _dynamicStates;

        bool createPipelineLayout();
        bool createGraphicsPipeline();

    public:
        Pipeline():
            _device(VK_NULL_HANDLE){}
        ~Pipeline(){};

        expected<void, PipeLineError> Init(
            VkDevice device, VkRenderPass RenderPass,
            const vector<VkPipelineShaderStageCreateInfo> &shaderStages);

        VkPipeline getPipeline() const { return _pipeline; }
        VkPipelineLayout getLayout() const { return _pipelineLayout; }
        
        bool isValid() const { 
            return _pipeline != VK_NULL_HANDLE && 
                   _pipelineLayout != VK_NULL_HANDLE; 
        }

};