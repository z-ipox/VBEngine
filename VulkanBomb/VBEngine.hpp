#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <ostream>
#include <string>
#include <expected>

#include "Vulkan/Core/Instance.h"
#include "Vulkan/Core/Device.h"
#include "Vulkan/Core/Surface.h"
#include "Vulkan/Core/Command.h"
#include "Vulkan/Core/SwapChain.h"

#include "Vulkan/Graphics/Pipeline.h"
#include "Vulkan/Graphics/RenderPass.h"
#include "Vulkan/Graphics/Shader.h"

#include "Common.h"

using namespace std;


enum class VBEngineError {
    VBEngineCreateWindowError,
    VBEngineCreateInstanceError,
    VBEngineCreateSurfaceError,
    VBEngineCreateDeviceError,
    VBEngineCreateSwapchainError,
    VBEngineCreateRenderPassError,
    VBEngineCreatePipelineError,
    VBEngineCreateShaderError,
    VBEngoneCreateCommandError
};

class VBEngine {
    private:
        string _programName;
        uint32_t _width, _height,
            _graphicsQueueFamilyIndex, _presentQueueFamilyIndex;
        uint32_t _currentFrame, _indexFrame, _maxFramesInFlight;
        VkInstance _instance;
        VkSurfaceKHR _surface;
        VkSurfaceFormatKHR _surfaceFormat;
        VkPresentModeKHR _presentMode;
        VkDevice _device;
        VkPhysicalDevice _physicalDevice;
        VkSwapchainKHR _swapchain;
        VkSurfaceCapabilitiesKHR _capabilities;
        VkRenderPass _renderPass;
        VkCommandPool _commandPool;
        VkPipeline _pipeline;
        VkShaderModule _shaderModule;
        vector<VkPipelineShaderStageCreateInfo> _shaderStages;
        GLFWwindow* _window;

        Device _vulkanBombDevice;
        Instance _vulkanBombInstance;
        Surface _vulkanBombSurface;
        SwapChain _vulkanBombSwapChain;
        Command _vulkanBombCommand;
        Pipeline _vulkanBombPipeline;
        RenderPass _vulkanBombRenderPass;
        Shader _vulkanBombShader;

    public:
        VBEngine() : _programName("VBEngine")
            , _width(800)
            , _height(600)
            , _window(nullptr)
            , _surface(VK_NULL_HANDLE)
            , _device(VK_NULL_HANDLE)
            , _swapchain(VK_NULL_HANDLE)
            , _currentFrame(0)
            , _maxFramesInFlight(2) {}
        ~VBEngine();

        VBEngine(const VBEngine&) = delete;
        VBEngine& operator=(const VBEngine&) = delete;

        expected<void, VBEngineError> Init(
            const string &programName, uint32_t width, uint32_t height,
            PresentMode presentMode = PresentMode::Unlimited, 
            SurfaceColorFormat surfaceColorFormat = SurfaceColorFormat::Standard);

        expected<void, VBEngineError> Update(bool showFPS = false);
        void Cleanup();

        GLFWwindow* getWindow() const { return _window; }
        uint32_t getWidth() { return _width; }
        uint32_t getHeight() { return _height; }
        
};
