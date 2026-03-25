#pragma once
#include "vulkan/vulkan.h"
#include <vector>
#include <expected>

using namespace std;

enum class CommandError{
    CommandDeviceError,
    CommandCreatePoolError,
    CommandCreateBuffersError,
    CommandCreateSyncObjectsError,
    CommandRecordBuffer
};

class Command {
    private:
        VkDevice _device;
        VkCommandPool _commandPool;
        vector<VkCommandBuffer> _commandBuffers;
        vector<VkSemaphore> _imageAvailableSemaphores;
        vector<VkSemaphore> _renderFinishedSemaphores;
        vector<VkFence> _inFlightFences;
        uint32_t _maxFrameInFlight, _graphicsQueueFamilyIndex;

        bool createCommandPool();
        bool createCommandBuffers();
        bool createSyncObjects();

    public:
        Command() : 
            _device(VK_NULL_HANDLE)
            , _commandPool(VK_NULL_HANDLE){}
        ~Command(){};

        expected<void, CommandError> Init(VkDevice device, uint32_t maxFrameInFlight, uint32_t graphicsQueueFamilyIndex);
        expected<void, CommandError> recordCommandBuffer(
            VkCommandBuffer commandBuffer, uint32_t frameIndex, 
            VkRenderPass renderPass, vector<VkFramebuffer>& framebuffers, 
            VkExtent2D extent, VkPipeline graphicsPipeline);

        VkCommandPool getCommandPool() const { return _commandPool; }
        const vector<VkCommandBuffer>& getCommandBuffers() const { return _commandBuffers; }
        const vector<VkSemaphore>& getImageAvailableSemaphores() const { return _imageAvailableSemaphores; }
        const vector<VkSemaphore>& getRenderFinishedSemaphores() const { return _renderFinishedSemaphores; }
        const vector<VkFence>& getInFlightFences() const { return _inFlightFences; }
};

