CC = g++

# Flags
LIBS = -lvulkan -lglfw -lc -lm -O3 -flto
CFLAGS = 
OUTPUT = vbengine

# Sources
S_PATH = VulkanBomb
SOURCES := main.cpp 
SOURCES += $(S_PATH)/VBengine.cpp 
SOURCES += $(S_PATH)/Vulkan/Core/Command.cpp $(S_PATH)/Vulkan/Core/Device.cpp $(S_PATH)/Vulkan/Core/Instance.cpp $(S_PATH)/Vulkan/Core/SwapChain.cpp 
SOuRCES += $(S_PATH)/Vulkan/Graphics/Buffer.cpp $(S_PATH)/Vulkan/Graphics/Pipeline.cpp $(S_PATH)/Vulkan/Graphics/Shader.cpp

# Build
all:
	$(CC) $(SOURCES) $(LIBS) $(CFLAGS) -o $(OUTPUT)

# Clean
clean:
	rm $(OUTPUT)