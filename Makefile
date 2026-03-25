CC = g++

# Flags
LIBS = -lvulkan -lglfw -lc -lm -O3 -flto
#LIBS = -lvulkan -lglfw -lc -lm -g
CFLAGS = -std=c++26
#CFLAGS = -std=c++26 -g
OUTPUT = vbengine

# Sources
S_PATH = VulkanBomb
SOURCES := main.cpp 
SOURCES += $(S_PATH)/VBEngine.cpp 

SOURCES += $(S_PATH)/Vulkan/Core/Command.cpp \
           $(S_PATH)/Vulkan/Core/Device.cpp \
           $(S_PATH)/Vulkan/Core/Instance.cpp \
           $(S_PATH)/Vulkan/Core/SwapChain.cpp \
           $(S_PATH)/Vulkan/Core/Surface.cpp

SOURCES += $(S_PATH)/Vulkan/Graphics/FrameBuffer.cpp \
           $(S_PATH)/Vulkan/Graphics/Pipeline.cpp \
           $(S_PATH)/Vulkan/Graphics/Shader.cpp \
           $(S_PATH)/Vulkan/Graphics/RenderPass.cpp
# Build
all:
	$(CC) $(SOURCES) $(LIBS) $(CFLAGS) -o $(OUTPUT)

# Clean
clean:
	rm $(OUTPUT)