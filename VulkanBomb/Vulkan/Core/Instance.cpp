#include "Instance.h"

expected<void, InstanceError> Instance::Init(const string &appName,
                    const vector<const char *> &extensions)
{
    _extensions = extensions;

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = appName.c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "VBEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_4;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(_extensions.size());
    createInfo.ppEnabledExtensionNames = _extensions.data();

    VkResult result = vkCreateInstance(&createInfo, nullptr, &_instance);

    if (result != VK_SUCCESS)
    {
        return unexpected(InstanceError::InstanceInitError);
    }
    return {};
}

// Instance::~Instance()
// {
//     if (_instance != VK_NULL_HANDLE)
//     {
//         vkDestroyInstance(_instance, nullptr);
//     }
// }
