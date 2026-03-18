#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <expected>
#include <string>
#include <iostream>
#include <fstream>
#include <set>

using namespace std;

enum class ShaderError{
    ShaderInitError,
    ShaderInitDeviceError,
    ShaderLoadReadError,
    ShaderCreateError,
    ShaderInvalidStage
};

class Shader {
    private:
        VkDevice _device;
        vector<VkPipelineShaderStageCreateInfo> _shaderStages;
        vector<VkShaderModule> _modules;
        VkShaderModule _shaderModule;
        VkShaderModuleCreateInfo _shaderModuleCreateInfo{};
        VkPipelineShaderStageCreateInfo _shaderStageInfo{};

        VkShaderModule createShaderModule(VkDevice device, vector<char> code);

    public:
        Shader() : 
            _device(VK_NULL_HANDLE){}
        ~Shader();

        expected<void, ShaderError> Init(VkDevice &device);
        expected<void, ShaderError> Load(const string& filePath, VkShaderStageFlagBits stage);

        const vector<VkPipelineShaderStageCreateInfo>& getStages() const { return _shaderStages; }
    
};

