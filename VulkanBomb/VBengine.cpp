#include "VBengine.hpp"
#include <string>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <iostream>
#include <ostream>
#include "Vulkan/Core/Instance.h"
#include "Vulkan/Core/Device.h"

using namespace std;

VBengine::VBengine(const std::string& programName, int width, int height)
{
	_programName = programName;
	_width = width;
	_height = height;
}

void VBengine::Init()
{
	auto window = glfwCreateWindow(_width, _height, _programName.c_str(), nullptr, nullptr);
	if (!window)
	{
		cout << "Failed to create window!" << endl;
		Cleanup();

	} else {
		
		uint32_t glfwExtensionCount = 0;
    	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
   	 	vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (!_vulkanInstance.Init(_programName, extensions)) {
        	throw runtime_error("Failed to create VkInstance");
    	}
    	cout << "Vulkan Instance created successfully\n";

		if (VkResult result = glfwCreateWindowSurface(_vulkanInstance.get(), _window,
			nullptr,
			&_surface
		) != VK_SUCCESS){
			throw runtime_error("Failed to create glfwWindowSurface");
		}
		cout << "WindowSurface created successfully\n";

		

		Run();
	
	}
}

void VBengine::Run()
{

}

void VBengine::Cleanup()
{
	glfwTerminate();
}

VBengine::~VBengine()
{
	/*vkDestroyInstance(instance, nullptr);
	vkDestroyDevice(device, nullptr);
	glfwDestroyWindow(window);
	*/
}