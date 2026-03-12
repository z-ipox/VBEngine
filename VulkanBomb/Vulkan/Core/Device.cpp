#include "Device.h"

using namespace std;

expected<void, device_error> Device::Init(VkInstance& instance){
    vkEnumeratePhysicalDevices(instance, &_deviceCount, nullptr);

    if (_deviceCount == 0){
       return unexpected(device_error::device_not_found);
    }

    vector<VkPhysicalDevice> devices(_deviceCount);

    vkEnumeratePhysicalDevices(instance, &_deviceCount, devices.data());
    _physicalDevice = devices[0];

    vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &_queueFamilyCount, nullptr);
    vector<VkQueueFamilyProperties> queueFamilies(_queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, 
        &_queueFamilyCount, queueFamilies.data());

    
    
        
}