#include "Command.h"

expected<void, CommandError> Command::Init(VkDevice device, uint32_t maxFrameInFlight, uint32_t graphicsQueueFamilyIndex)
{
    if (device == VK_NULL_HANDLE)
        return unexpected(CommandError::CommandDeviceError);

    _device = device;
    _maxFrameInFlight = maxFrameInFlight;
    _graphicsQueueFamilyIndex = graphicsQueueFamilyIndex;

    if (createCommandPool() == false)
        return unexpected(CommandError::CommandCreatePoolError);
    if (createCommandBuffers() == false)
        return unexpected(CommandError::CommandCreateBuffersError);
    if (createSyncObjects() == false)
        return unexpected(CommandError::CommandCreateSyncObjectsError);
    
    return {};
}

bool Command::createCommandPool()
{
    VkCommandPoolCreateInfo commandPoolCreateInfo{};
    commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandPoolCreateInfo.queueFamilyIndex = _graphicsQueueFamilyIndex;

    if (vkCreateCommandPool(_device, &commandPoolCreateInfo, nullptr, &_commandPool) != VK_SUCCESS) {
        return false;
    } 
    return true;
}

bool Command::createCommandBuffers()
{
    _commandBuffers.resize(_maxFrameInFlight);
    VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.commandPool = _commandPool;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateInfo.commandBufferCount = static_cast<uint32_t>(_commandBuffers.size());

    if (vkAllocateCommandBuffers(_device, &commandBufferAllocateInfo, _commandBuffers.data()) != VK_SUCCESS) {
        return false;
    }
    return true;
}

bool Command::createSyncObjects()
{
    _imageAvailableSemaphores.resize(_maxFrameInFlight);
    _renderFinishedSemaphores.resize(_maxFrameInFlight);
    _inFlightFences.resize(_maxFrameInFlight);

    VkSemaphoreCreateInfo semaphoreCreateInfo{};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceCreateInfo{};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (int i = 0; i < _maxFrameInFlight; i++) {
        if (vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(_device, &fenceCreateInfo, nullptr, &_inFlightFences[i]) != VK_SUCCESS) {
            return false;
        }
    }
    return true;
}

// Command::~Command() {
//     if (_device != VK_NULL_HANDLE) {
//         for (int i = 0; i < _maxFrameInFlight; i++) {
//             vkDestroySemaphore(_device, _imageAvailableSemaphores[i], nullptr);
//             vkDestroySemaphore(_device, _renderFinishedSemaphores[i], nullptr);
//             vkDestroyFence(_device, _inFlightFences[i], nullptr);
//         }
        
//         if (_commandPool != VK_NULL_HANDLE) {
//             vkDestroyCommandPool(_device, _commandPool, nullptr);
//         }
//     }
// }
