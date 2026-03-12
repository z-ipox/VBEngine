#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <string>

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

        bool Init(VkInstance& instance);


        VkDevice getDevice() const { return _device; }
        VkPhysicalDevice getPhDevice() const { return _physicalDevice; }

};


