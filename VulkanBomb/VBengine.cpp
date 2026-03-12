#include "VBengine.hpp"


using namespace std;

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

		Instance _instance;

		if (!_instance.Init(_programName, extensions)) {
        	throw runtime_error("Failed to create VkInstance");
    	}
    	cout << "Vulkan Instance created successfully\n";

		if (!glfwCreateWindowSurface(_instance.get(), _window,
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