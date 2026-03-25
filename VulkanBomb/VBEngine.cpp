#include "VBEngine.hpp"

using namespace std;

expected<void, VBEngineError> VBEngine::Init(
	const string &programName, uint32_t width, uint32_t height,
	PresentMode presentMode, SurfaceColorFormat surfaceColorFormat)
{
	if (!glfwInit()) return unexpected(VBEngineError::VBEngineCreateWindowError);

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

	_width = width;
	_height = height;
	_programName = programName;

	_window = glfwCreateWindow(_width, _height, _programName.c_str(), nullptr, nullptr);
	if (!_window) {
		Cleanup();
		return unexpected(VBEngineError::VBEngineCreateWindowError);
	}

	// 1. Instance
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (!_vulkanBombInstance.Init(_programName, extensions)) {
		Cleanup();
		return unexpected(VBEngineError::VBEngineCreateInstanceError);
	}
	_instance = _vulkanBombInstance.getInstance();

	// 2. Surface
	if (!_vulkanBombSurface.Init(_instance, _window)) {
		Cleanup();
		return unexpected(VBEngineError::VBEngineCreateSurfaceError);
	}
	_surface = _vulkanBombSurface.getSurface();

	// 3. Device
    if (!_vulkanBombDevice.Init(_instance, _surface)) {
		Cleanup();
        return unexpected(VBEngineError::VBEngineCreateDeviceError);
    }
    _device = _vulkanBombDevice.getDevice();
    _physicalDevice = _vulkanBombDevice.getPhysicalDevice();
	_graphicsQueue = _vulkanBombDevice.getGraphicsQueue();
	_presentQueue = _vulkanBombDevice.getPresentQueue();
	_graphicsQueueFamilyIndex = _vulkanBombDevice.getGraphicsQueueFamilyIndex();
	_presentQueueFamilyIndex = _vulkanBombDevice.getPresentQueueFamalyIndex();

	// 4. Update Surface Caps
    if (!_vulkanBombSurface.UpdateCapabilities(_physicalDevice, presentMode, surfaceColorFormat)) {
		Cleanup();
        return unexpected(VBEngineError::VBEngineCreateSurfaceError);
    }
	_surfaceFormat = _vulkanBombSurface.getSurfaceFormat();
	_presentMode = _vulkanBombSurface.getPresentMode();
	_capabilities = _vulkanBombSurface.getCapabilities();

	// 5. SwapChain
	if (!_vulkanBombSwapChain.Init(_device, _surface, _window, _surfaceFormat, _presentMode, 
		                            _capabilities, _graphicsQueueFamilyIndex, _presentQueueFamilyIndex)) {
		Cleanup();
		return unexpected(VBEngineError::VBEngineCreateSwapchainError);
	}
	_swapchain = _vulkanBombSwapChain.getSwapchain();
	_extent2D = _vulkanBombSwapChain.getExtent2D();
	_swapchainImageViews = _vulkanBombSwapChain.getSwapChainImageViews();
	_imageCount = static_cast<uint32_t>(_swapchainImageViews.size());

	// 6. RenderPass
	if (!_vulkanBombRenderPass.Init(_surfaceFormat, _device)) {
		Cleanup();
		return unexpected(VBEngineError::VBEngineCreateRenderPassError);
	}
	_renderPass = _vulkanBombRenderPass.getRenderPass();

	// 7. Shaders
	if (!_vulkanBombShader.Init(_device)) {
		Cleanup();
		return unexpected(VBEngineError::VBEngineCreateShaderError);
	}
	if (!_vulkanBombShader.Load("shader/vert.spv", VK_SHADER_STAGE_VERTEX_BIT) ||
		!_vulkanBombShader.Load("shader/frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT)) {
		Cleanup();
		return unexpected(VBEngineError::VBEngineCreateShaderError);
	}
	_shaderStages = _vulkanBombShader.getStages();

	// 8. Pipeline
	if (!_vulkanBombPipeline.Init(_device, _renderPass, _shaderStages)) {
		Cleanup();
		return unexpected(VBEngineError::VBEngineCreatePipelineError);
	}
	_pipeline = _vulkanBombPipeline.getPipeline();

	// 9. FrameBuffer
	if (!_vulkanBombFrameBuffer.Init(_imageCount, _swapchainImageViews, _renderPass, _device, _extent2D)) {
		Cleanup();
		return unexpected(VBEngineError::VBEngineCreateFrameBufferError);
	}
	_framebuffers = _vulkanBombFrameBuffer.getFrameBuffers();

	// 10. Command
	_maxFramesInFlight = 2;
	if (!_vulkanBombCommand.Init(_device, _maxFramesInFlight, _graphicsQueueFamilyIndex)) {
		Cleanup();
		return unexpected(VBEngineError::VBEngineCreateCommandError);
	}
	
	_commandBuffers = _vulkanBombCommand.getCommandBuffers();
	_imageAvailableSemaphores = _vulkanBombCommand.getImageAvailableSemaphores();
	_renderFinishedSemaphores = _vulkanBombCommand.getRenderFinishedSemaphores();
	_inFlightFences = _vulkanBombCommand.getInFlightFences();

	return {};
}


void VBEngine::Cleanup()
{
    if (_device != VK_NULL_HANDLE) {
        vkDeviceWaitIdle(_device);
    }

    if (_device != VK_NULL_HANDLE) {
        for (size_t i = 0; i < _imageAvailableSemaphores.size(); i++) {
            if (_imageAvailableSemaphores[i] != VK_NULL_HANDLE) 
                vkDestroySemaphore(_device, _imageAvailableSemaphores[i], nullptr);
            if (_renderFinishedSemaphores[i] != VK_NULL_HANDLE) 
                vkDestroySemaphore(_device, _renderFinishedSemaphores[i], nullptr);
            if (_inFlightFences[i] != VK_NULL_HANDLE) 
                vkDestroyFence(_device, _inFlightFences[i], nullptr);
        }

        VkCommandPool pool = _vulkanBombCommand.getCommandPool();
        if (pool != VK_NULL_HANDLE) {
            vkDestroyCommandPool(_device, pool, nullptr);
        }
        for (auto fb : _framebuffers) {
            if (fb != VK_NULL_HANDLE) vkDestroyFramebuffer(_device, fb, nullptr);
        }
        if (_pipeline != VK_NULL_HANDLE) {
            vkDestroyPipeline(_device, _pipeline, nullptr);
        }
        VkPipelineLayout layout = _vulkanBombPipeline.getLayout();
        if (layout != VK_NULL_HANDLE) {
            vkDestroyPipelineLayout(_device, layout, nullptr);
        }
        if (_renderPass != VK_NULL_HANDLE) {
            vkDestroyRenderPass(_device, _renderPass, nullptr);
        }
        for (auto iv : _swapchainImageViews) {
            if (iv != VK_NULL_HANDLE) vkDestroyImageView(_device, iv, nullptr);
        }
        if (_swapchain != VK_NULL_HANDLE) {
            vkDestroySwapchainKHR(_device, _swapchain, nullptr);
        }
        vkDestroyDevice(_device, nullptr);
        _device = VK_NULL_HANDLE;
    }

    if (_instance != VK_NULL_HANDLE) {
        if (_surface != VK_NULL_HANDLE) {
            vkDestroySurfaceKHR(_instance, _surface, nullptr);
        }
        vkDestroyInstance(_instance, nullptr);
        _instance = VK_NULL_HANDLE;
    }

    if (_window) {
        glfwDestroyWindow(_window);
    }
    glfwTerminate();
}

VBEngine::~VBEngine() {
    Cleanup();
}
