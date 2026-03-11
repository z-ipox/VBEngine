#include "VBengine.hpp"
#include <string>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <iostream>
#include <ostream>
#include <GLFW/glfw3.h>

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