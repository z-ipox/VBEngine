#include <vector>
#include "Device.h"
#include <string>

using namespace std;

Device::Init(VkInstance& instance){
    vkEnumeratePhysicalDevices(instance, &_deviceCount, nullptr)
    if (_deviceCount == 0){
        throw runtime_error("No Vulkan capable devices found!");
    }
    vector<VkPhysicalDevice> devices(_deviceCount);
    vkEnumeratePhysicalDevices(instance, &_deviceCount, devices.data());
    _physicalDevice = devices[0];

    vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &_queueFamilyCount, nullptr);
    vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, 
        &_queueFamilyCount, queueFamilies.data());
    
        
}