#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <expected>
#include <string>

enum class device_error
{
    device_not_found
};

class Device {

    private:
        VkDevice _device;
        VkPhysicalDevice _physicalDevice; 
        uint32_t _deviceCount;
        uint32_t _queueFamilyCount;

        bool findQueueFamilies();
        
    public:

        Device() : _device(VK_NULL_HANDLE)
        , _physicalDevice(VK_NULL_HANDLE)
        , _deviceCount(0)
        , _queueFamilyCount(0) {}

        ~Device();

        std::expected<void, device_error> Init(VkInstance& instance);


        VkDevice getDevice() const { return _device; }
        VkPhysicalDevice getPhDevice() const { return _physicalDevice; }

};


