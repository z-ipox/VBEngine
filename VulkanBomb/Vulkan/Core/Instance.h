#pragma once
#include <iostream>
#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include <expected>

using namespace std;

enum class InstanceError{
    InstanceInitError
};

class Instance {
private:
    VkInstance _instance;
    vector<const char*> _extensions;

public:
    Instance() : _instance(VK_NULL_HANDLE) {}

    ~Instance(){};

    Instance(const Instance&) = delete;
    Instance& operator=(const Instance&) = delete;

    expected<void, InstanceError> Init(const string& appName,
              const vector<const char*>& extensions = {});

    VkInstance getInstance() const { return _instance; }

    const vector<const char*>& getExtensions() const { return _extensions; }

    bool isValid() const { return _instance != VK_NULL_HANDLE; }
};