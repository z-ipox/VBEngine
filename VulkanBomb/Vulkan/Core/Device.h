#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <expected>
#include <string>
#include <set>

using namespace std;

enum class DeviceError
{
    DeviceNotFound,
    DeviceNotFoundQueueFamilies,
    DeviceInvalidSurface,
    DeviceFiledCreateLogicDevice,
};

class Device {

    private:
    
        VkDevice _device;
        VkPhysicalDevice _physicalDevice; 
        VkSurfaceKHR _surface;
        vector<VkQueueFamilyProperties> _queueFamilies;
        vector<VkDeviceQueueCreateInfo> _queueCreateInfos;
        VkQueue _graphicsQueue,
                _presentQueue;
        uint32_t _deviceCount,
                 _queueFamilyCount;
        int _graphicsQueueFamilyIndex,
            _presentQueueFamilyIndex;

        bool findPhysicalDevice(VkInstance &instance);
        bool findQueueFamiliesIndex();
        bool createLogicDevice();

    public:

        Device() : _device(VK_NULL_HANDLE)
        , _physicalDevice(VK_NULL_HANDLE)
        , _deviceCount(0)
        , _queueFamilyCount(0)
        , _graphicsQueueFamilyIndex(-1)
        , _presentQueueFamilyIndex(-1) {}

        ~Device();

        expected<void, DeviceError> Init(VkInstance& instance, VkSurfaceKHR *surface);

        VkDevice getDevice() const { return _device; }
        VkPhysicalDevice getPhysicalDevice() const { return _physicalDevice; }
        VkQueue getGraphicsQueue() const { return _graphicsQueue; }
        VkQueue getPresentQueue() const { return _presentQueue; }
        uint32_t getDeviceCount() const { return _deviceCount; }
        uint32_t getQueueFamilyCount() const { return _queueFamilyCount; }
        int getGraphicsQueueFamilyIndex() const { return _graphicsQueueFamilyIndex; }
        int getPresentQueueFamalyIndex() const { return _presentQueueFamilyIndex; }

};


