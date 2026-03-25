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
#include "Vulkan/Graphics/FrameBuffer.h"

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
    VBEngineCreateCommandError,
    VBEngineCreateFrameBufferError
};

class VBEngine {
    private:
        string _programName;
        uint32_t _width, _height,
            _graphicsQueueFamilyIndex, _presentQueueFamilyIndex,
            _currentFrame, _indexFrame, _maxFramesInFlight,
            _imageCount;
        VkInstance _instance;
        VkSurfaceKHR _surface;
        VkSurfaceFormatKHR _surfaceFormat;
        VkPresentModeKHR _presentMode;
        VkQueue _graphicsQueue;
        VkQueue _presentQueue;
        VkDevice _device;
        VkPhysicalDevice _physicalDevice;
        VkSwapchainKHR _swapchain;
        VkSurfaceCapabilitiesKHR _capabilities;
        VkRenderPass _renderPass;
        VkCommandPool _commandPool;
        VkPipeline _pipeline;
        VkShaderModule _shaderModule;
        vector<VkPipelineShaderStageCreateInfo> _shaderStages;
        VkExtent2D _extent2D;
        vector<VkImage> _swapchainImages;
        vector<VkImageView> _swapchainImageViews;
        vector<VkSemaphore> _imageAvailableSemaphores;
        vector<VkSemaphore> _renderFinishedSemaphores;
        vector<VkFence> _inFlightFences;
        vector<VkCommandBuffer> _commandBuffers;
        vector<VkFramebuffer> _framebuffers;
        GLFWwindow* _window;

        Device _vulkanBombDevice;
        Instance _vulkanBombInstance;
        Surface _vulkanBombSurface;
        SwapChain _vulkanBombSwapChain;
        Command _vulkanBombCommand;
        Pipeline _vulkanBombPipeline;
        FrameBuffer _vulkanBombFrameBuffer;
        RenderPass _vulkanBombRenderPass;
        Shader _vulkanBombShader;

    public:
        VBEngine() : _programName("VBEngine")
            , _width(800)
            , _height(600)
            , _instance(VK_NULL_HANDLE),
              _device(VK_NULL_HANDLE),
              _surface(VK_NULL_HANDLE),
              _swapchain(VK_NULL_HANDLE),
              _renderPass(VK_NULL_HANDLE),
              _pipeline(VK_NULL_HANDLE),
              _commandPool(VK_NULL_HANDLE),
              _window(nullptr)
            , _currentFrame(0)
            , _maxFramesInFlight(2) {}
        ~VBEngine();

        VBEngine(const VBEngine&) = delete;
        VBEngine& operator=(const VBEngine&) = delete;

        expected<void, VBEngineError> Init(
            const string &programName, uint32_t width, uint32_t height,
            PresentMode presentMode = PresentMode::Unlimited, 
            SurfaceColorFormat surfaceColorFormat = SurfaceColorFormat::Standard);

        template<bool ShowFPS = false>
        expected<void, VBEngineError> RunTriangle2D()
        {
            [[maybe_unused]] double lastTime = 0.0;
            [[maybe_unused]] uint32_t frameCount = 0;
            
            if constexpr (ShowFPS) {
                lastTime = glfwGetTime();
            }   

            while (!glfwWindowShouldClose(_window)) {
                glfwPollEvents();

                if constexpr (ShowFPS) {
                    frameCount++;
                    double currentTime = glfwGetTime();
                    if (currentTime - lastTime >= 1.0) {
                        printf("FPS: %d\n", frameCount);
                        frameCount = 0;
                        lastTime = currentTime;
                    }
                }

                vkWaitForFences(_device, 1, &_inFlightFences[_currentFrame], VK_TRUE, UINT64_MAX);
                vkResetFences(_device, 1, &_inFlightFences[_currentFrame]);

                uint32_t imageIndex;
                VkResult result = vkAcquireNextImageKHR(_device, _swapchain, UINT64_MAX,
                                                        _imageAvailableSemaphores[_currentFrame],
                                                        VK_NULL_HANDLE, &imageIndex);
                if (result != VK_SUCCESS) continue;
                                                        
                vkResetCommandBuffer(_commandBuffers[_currentFrame], 0);
                _vulkanBombCommand.recordCommandBuffer(_commandBuffers[_currentFrame], imageIndex, _renderPass, _framebuffers, _extent2D, _pipeline);

                VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
                VkSubmitInfo submitInfo = {};
                submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
                submitInfo.waitSemaphoreCount = 1;
                submitInfo.pWaitSemaphores = &_imageAvailableSemaphores[_currentFrame];
                submitInfo.pWaitDstStageMask = waitStages;
                submitInfo.commandBufferCount = 1;
                submitInfo.pCommandBuffers = &_commandBuffers[_currentFrame];
                submitInfo.signalSemaphoreCount = 1;
                submitInfo.pSignalSemaphores = &_renderFinishedSemaphores[_currentFrame];

                if (vkQueueSubmit(_graphicsQueue, 1, &submitInfo, _inFlightFences[_currentFrame]) != VK_SUCCESS) {
                    break;
                }

                VkPresentInfoKHR presentInfo = {};
                presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
                presentInfo.waitSemaphoreCount = 1;
                presentInfo.pWaitSemaphores = &_renderFinishedSemaphores[_currentFrame];
                presentInfo.swapchainCount = 1;
                presentInfo.pSwapchains = &_swapchain;
                presentInfo.pImageIndices = &imageIndex;

                vkQueuePresentKHR(_presentQueue, &presentInfo);

                _currentFrame = (_currentFrame + 1) % _maxFramesInFlight;
            }
            return {};
        }

        void Cleanup();

        GLFWwindow* getWindow() const { return _window; }
        uint32_t getWidth() { return _width; }
        uint32_t getHeight() { return _height; }
        
};
