#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <string>

class Instance {
private:
    VkInstance _instance;
    std::vector<const char*> _extensions;

public:
    Instance() : _instance(VK_NULL_HANDLE) {}

    ~Instance();

    Instance(const Instance&) = delete;
    Instance& operator=(const Instance&) = delete;

    bool Init(const std::string& appName,
              uint32_t appVersion = VK_MAKE_VERSION(1, 0, 0),
              const std::vector<const char*>& extensions = {});

    VkInstance get() const { return _instance; }
    const std::vector<const char*>& getExtensions() const { return _extensions; }

    bool isValid() const { return _instance != VK_NULL_HANDLE; }
};