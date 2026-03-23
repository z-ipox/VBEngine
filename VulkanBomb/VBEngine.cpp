#include "VBEngine.hpp"

using namespace std;

expected<void, VBEngineError> VBEngine::Init(
	const string &programName, int width, int height,
	PresentMode presentMode, SurfaceColorFormat surfaceColorFormat)
{
	auto window = glfwCreateWindow(_width, _height, _programName.c_str(), nullptr, nullptr);
	if (!window)
	{
		Cleanup();
		return unexpected(VBEngineError::VBEngineCreateWindowError);
	} 
		
	uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
 	vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (!_vulkanBombInstance.Init(_programName, extensions)) {
		Cleanup();
    	return unexpected(VBEngineError::VBEngineCreateInstanceError);
	}

	_instance = _vulkanBombInstance.getInstance();

	if (!_vulkanBombSurface.Init(
		_instance, window, presentMode, surfaceColorFormat)){
		Cleanup();
		return unexpected(VBEngineError::VBEngineCreateSurfaceError);
	}

	_surface = _vulkanBombSurface.getSurface();
	_surfaceFormat = _vulkanBombSurface.getSurfaceFormat();
	_presentMode = _vulkanBombSurface.getPresentMode();
	_capabilities = _vulkanBombSurface.getCapabilities();
	
	if (!_vulkanBombDevice.Init(_instance, _surface)){
		Cleanup();
		return unexpected(VBEngineError::VBEngineCreateDeviceError);
	}

	_device = _vulkanBombDevice.getDevice();
	_graphicsQueueFamilyIndex = _vulkanBombDevice.getGraphicsQueueFamilyIndex();
	_presentQueueFamilyIndex = _vulkanBombDevice.getPresentQueueFamalyIndex();

	if (!_vulkanBombSwapChain.Init(
		_device, _surface, _surfaceFormat,
		_presentMode, _capabilities, 
		_graphicsQueueFamilyIndex, _presentQueueFamilyIndex))
	{
		Cleanup();
		return unexpected(VBEngineError::VBEngineCreateSwapchainError);
	}

	if (!_vulkanBombRenderPass.Init(_surfaceFormat, _device)){
		Cleanup();
		return unexpected(VBEngineError::VBEngineCreateRenderPassError);
	}

	_renderPass = _vulkanBombRenderPass.getRenderPass();
	
	if (!_vulkanBombShader.Init(_device)){
		Cleanup();
		return unexpected(VBEngineError::VBEngineCreateShaderError);
	}

	_shaderStages = _vulkanBombShader.getStages();

	if (!_vulkanBombPipeline.Init(_device, _renderPass, _shaderStages)){
		Cleanup();
		return unexpected(VBEngineError::VBEngineCreatePipelineError);
	}
	if (!_vulkanBombCommand.Init(_device, _maxFramesInFlight, _graphicsQueueFamilyIndex)){
		Cleanup();
		return unexpected(VBEngineError::VBEngoneCreateCommandError);
	}
	
	return {};
}

expected<void, VBEngineError> VBEngine::Update(bool showFPS)
{
	return {};
}

void VBEngine::Cleanup()
{
    if (_device != VK_NULL_HANDLE) {
        vkDeviceWaitIdle(_device);
    }

    if (_surface != VK_NULL_HANDLE) vkDestroySurfaceKHR(_instance, _surface, nullptr);
    if (_device != VK_NULL_HANDLE) vkDestroyDevice(_device, nullptr);
    if (_instance != VK_NULL_HANDLE) vkDestroyInstance(_instance, nullptr);
	if (_renderPass != VK_NULL_HANDLE) vkDestroyRenderPass(_device, _renderPass, nullptr);
	if (_commandPool != VK_NULL_HANDLE) vkDestroyCommandPool(_device, _commandPool, nullptr);
	if (_pipeline != VK_NULL_HANDLE) vkDestroyPipeline(_device, _pipeline, nullptr);
	if (_shaderModule != VK_NULL_HANDLE) vkDestroyShaderModule(_device, _shaderModule, nullptr);
	
    if (_window) {
        glfwDestroyWindow(_window);
    }
    glfwTerminate();
}

VBEngine::~VBEngine() {
    Cleanup();
}
