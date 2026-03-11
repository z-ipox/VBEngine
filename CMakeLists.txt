cmake_minimum_required(VERSION 4.1)
project(imgui_example_glfw_vulkan C CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

set(CMAKE_CXX_STANDARD 26)
add_definitions(-DVK_PROTOTYPES)

# Путь к GLFW
set(GLFW_DIR "/mnt/1337/glfw" CACHE PATH "Path to GLFW repository")

# Путь к ImGui - УЖЕ ПРАВИЛЬНЫЙ!
set(IMGUI_DIR "/mnt/1337/Projects/imgui" CACHE PATH "Path to Dear ImGui")

# Проверяем существование папок
if(NOT EXISTS ${GLFW_DIR})
  message(FATAL_ERROR "GLFW directory not found at: ${GLFW_DIR}")
endif()

if(NOT EXISTS ${IMGUI_DIR})
  message(FATAL_ERROR "ImGui directory not found at: ${IMGUI_DIR}")
endif()

# Настройки GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# Добавляем GLFW
add_subdirectory(${GLFW_DIR} glfw_build EXCLUDE_FROM_ALL)

# Include directories - ВАЖНО: добавляем правильные пути!
include_directories(
        ${GLFW_DIR}/include
        ${GLFW_DIR}/deps
        ${IMGUI_DIR}              # Для основных заголовков ImGui
        ${IMGUI_DIR}/backends     # Для бэкендов ImGui
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Vulkan
find_package(Vulkan REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)
include_directories(${Vulkan_INCLUDE_DIRS})

# Находим все .cpp файлы в текущей директории проекта
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Явно указываем ВСЕ необходимые файлы ImGui с АБСОЛЮТНЫМИ путями
set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
        VulkanBomb/Vulkan/Core/Instance.cpp
        VulkanBomb/Vulkan/Core/Instance.h
        VulkanBomb/Vulkan/Core/SwapChain.cpp
        VulkanBomb/Vulkan/Core/SwapChain.h
        VulkanBomb/Vulkan/Core/Device.cpp
        VulkanBomb/Vulkan/Core/Device.h
        VulkanBomb/Vulkan/Core/Command.cpp
        VulkanBomb/Vulkan/Core/Command.h
        VulkanBomb/Vulkan/Graphics/Shader.cpp
        VulkanBomb/Vulkan/Graphics/Shader.h
        VulkanBomb/Vulkan/Graphics/Buffer.cpp
        VulkanBomb/Vulkan/Graphics/Buffer.h
        VulkanBomb/Vulkan/Graphics/Pipeline.cpp
        VulkanBomb/VBengine.hpp
)

# Проверяем существование всех файлов ImGui
message(STATUS "Checking ImGui files:")
foreach(IMGUI_FILE ${IMGUI_SOURCES})
  if(EXISTS ${IMGUI_FILE})
    message(STATUS "  Found: ${IMGUI_FILE}")
  else()
    message(FATAL_ERROR "  NOT FOUND: ${IMGUI_FILE}")
  endif()
endforeach()

# Создаем исполняемый файл со ВСЕМИ исходниками
add_executable(example_glfw_vulkan ${SOURCES} ${IMGUI_SOURCES})

# Линкуем библиотеки
target_link_libraries(example_glfw_vulkan
        glfw
        Vulkan::Vulkan
        m
        pthread
        dl
        X11
)