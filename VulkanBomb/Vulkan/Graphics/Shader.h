#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <expected>
#include <string>
#include <set>

using namespace std;

// struct ShaderStage {
//     VkShaderModule module;
//     VkShaderStageFlagBits stage;
//     std::string entryPoint = "main";
// };

enum class ShaderError{
    ShaderInitError,
};

class Shader {
    private:
        VkDevice _device;
        vector<VkPipelineShaderStageCreateInfo> _shaderStages;
        vector<VkShaderModule> _modules;

    public:
        Shader() : 
            _device(VK_NULL_HANDLE){}
        ~Shader();

        expected<void, ShaderError> Init(VkDevice &device);
        bool Load(const string& filePath, VkShaderStageFlagBits stage);

        const vector<VkPipelineShaderStageCreateInfo>& getStages() const { return _shaderStages; }
    
};

