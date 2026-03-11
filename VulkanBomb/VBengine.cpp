#include "VBengine.hpp"
#include <string>
#include <GLFW/glfw3.h>
#include <vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <iostream>
#include <ostream>
#include <GLFW/glfw3.h>

using namespace std;

class VBengine{
private:
	static string _programName;
	static int _width, _height;
public:

	VBengine(string programName, int width, int height){
		_programName = programName;
		_width = width;
		_height = height;
	}

	void Init() {
		auto window = glfwCreateWindow(_width, _height, _programName.c_str(), nullptr, nullptr);
		if (!window) {
			cout << "Failed to create window!" << endl;
			Cleanup();
		}

	}

	void Run() {

	}

	void Cleanup() {
		glfwTerminate();
	}

	~VBengine(){
		/*vkDestroyInstance(instance, nullptr);
		vkDestroyDevice(device, nullptr);
		glfwDestroyWindow(window);
		*/
	}
};