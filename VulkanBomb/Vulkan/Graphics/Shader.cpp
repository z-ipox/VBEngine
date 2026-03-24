#include "Shader.h"

expected<void, ShaderError> Shader::Init(VkDevice device)
{
    if (device == VK_NULL_HANDLE){
        return unexpected(ShaderError::ShaderInitDeviceError);
    }
    return {};
}

expected<void, ShaderError> Shader::Load(const string& filePath, VkShaderStageFlagBits stage)
{
    ifstream file(filePath, ios::ate | ios::binary);
    if (!file.is_open()) {
        return unexpected(ShaderError::ShaderLoadReadError);
    }
    if (stage == VK_SHADER_STAGE_ALL){
        return unexpected(ShaderError::ShaderInvalidStage);
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    _shaderModule = createShaderModule(buffer);
    if ( _shaderModule == VK_NULL_HANDLE){
        return unexpected(ShaderError::ShaderCreateError);
    }

    VkPipelineShaderStageCreateInfo shaderStageInfo{};
    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.stage = stage;
    shaderStageInfo.module = _shaderModule;
    shaderStageInfo.pName = "main";

    _shaderStages.push_back(shaderStageInfo);
    return {};
}

VkShaderModule Shader::createShaderModule(vector<char> &code) 
{
    VkShaderModuleCreateInfo shaderModuleCreateInfo{};
    shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleCreateInfo.codeSize = code.size();
    shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    if (vkCreateShaderModule(_device, &shaderModuleCreateInfo, nullptr, &_shaderModule) != VK_SUCCESS) {
        return VK_NULL_HANDLE;
    }
    return _shaderModule;
}

// Shader::~Shader(){
//     for (auto module : _modules) {
//         if (module != VK_NULL_HANDLE && _device != VK_NULL_HANDLE) {
//             vkDestroyShaderModule(_device, module, nullptr);
//         }
//     }
// }