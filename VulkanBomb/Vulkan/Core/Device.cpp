#include "Device.h"

using namespace std;

expected<void, DeviceError> Device::Init(VkInstance& instance, VkSurfaceKHR *surface)
{
    if (surface == nullptr || *surface == VK_NULL_HANDLE) {
        return unexpected(DeviceError::DeviceInvalidSurface);
    }

    _surface = *surface;  

    if (findPhysicalDevice(instance) == false){
        return unexpected(DeviceError::DeviceNotFound);
    }
    if (findQueueFamiliesIndex() == false){
        return unexpected(DeviceError::DeviceNotFoundQueueFamilies);
    }
    if (createLogicDevice() == false){
        return unexpected(DeviceError::DeviceFiledCreateLogicDevice);
    }

    vkGetDeviceQueue(_device, _graphicsQueueFamilyIndex, 0, &_graphicsQueue);
    vkGetDeviceQueue(_device, _presentQueueFamilyIndex, 0, &_presentQueue);
    

}

bool Device::findPhysicalDevice(VkInstance &instance){
    vkEnumeratePhysicalDevices(instance, &_deviceCount, nullptr);

    if (_deviceCount == 0){
       return false;
    }

    vector<VkPhysicalDevice> devices(_deviceCount);

    vkEnumeratePhysicalDevices(instance, &_deviceCount, devices.data());
    _physicalDevice = devices[0];

    vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &_queueFamilyCount, nullptr);

    _queueFamilies.resize(_queueFamilyCount);

    vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, 
    &_queueFamilyCount, _queueFamilies.data());
    
    return true;
}

bool Device::findQueueFamiliesIndex(){
    for (uint32_t i = 0; i < _queueFamilyCount; i++) {
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(_physicalDevice, i, _surface, &presentSupport);

        if (presentSupport) {
            _presentQueueFamilyIndex = i;
        }

        if (_queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            _graphicsQueueFamilyIndex = i;
        }
    }
    if (_graphicsQueueFamilyIndex == -1 || _presentQueueFamilyIndex == -1) {
        return false;
    }
    return true;
}

bool Device::createLogicDevice(){
    set<uint32_t> uniqueQueueFamilies = {
        static_cast<uint32_t>(_graphicsQueueFamilyIndex),
        static_cast<uint32_t>(_presentQueueFamilyIndex)
    };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        _queueCreateInfos.push_back(queueCreateInfo);
    }

    const char* deviceExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = nullptr;
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(_queueCreateInfos.size());
    deviceCreateInfo.pQueueCreateInfos = _queueCreateInfos.data();
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    deviceCreateInfo.enabledExtensionCount = 1;
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions;

    if (vkCreateDevice(_physicalDevice, &deviceCreateInfo, nullptr, &_device) != VK_SUCCESS)
    {
        return false;
    }
    
    return true;
}




