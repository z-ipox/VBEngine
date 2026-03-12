#pragma once
#include <iostream>
#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include <expected>

enum class instance_error{
    init_error
};

class Instance {
private:
    VkInstance _instance;
    std::vector<const char*> _extensions;

public:
    Instance() : _instance(VK_NULL_HANDLE) {}

    ~Instance();

    Instance(const Instance&) = delete;
    Instance& operator=(const Instance&) = delete;

    std::expected<void, instance_error> Init(const std::string& appName,
              const std::vector<const char*>& extensions = {});

    VkInstance get() const { return _instance; }

    const std::vector<const char*>& getExtensions() const { return _extensions; }

    bool isValid() const { return _instance != VK_NULL_HANDLE; }
};