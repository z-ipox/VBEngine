#pragma once
#include <vulkan/vulkan.h>

enum class PresentMode {
    VSync            =   VK_PRESENT_MODE_FIFO_KHR,
    VSyncAdaptive    =   VK_PRESENT_MODE_FIFO_RELAXED_KHR,
    TripleBuffered   =   VK_PRESENT_MODE_MAILBOX_KHR,
    Unlimited        =   VK_PRESENT_MODE_IMMEDIATE_KHR
};

enum class SurfaceColorFormat {
    Standard    =    VK_FORMAT_B8G8R8A8_SRGB,
    Linear      =    VK_FORMAT_B8G8R8A8_UNORM,
    HDR         =    VK_FORMAT_R16G16B16A16_SFLOAT,
    HDR10       =    VK_FORMAT_A2B10G10R10_UNORM_PACK32,
    RGB_Order   =    VK_FORMAT_R8G8B8A8_SRGB
};

enum class ShaderStage {
    Vertex             =     VK_SHADER_STAGE_VERTEX_BIT,
    Fragment           =     VK_SHADER_STAGE_FRAGMENT_BIT,
    Geometry           =     VK_SHADER_STAGE_GEOMETRY_BIT,
    TessControl        =     VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
    TessEvaluation     =     VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
    Compute            =     VK_SHADER_STAGE_COMPUTE_BIT,

    // Path Tracing - Ray Tracing

    RayGen             =     VK_SHADER_STAGE_RAYGEN_BIT_KHR,
    RayMiss            =     VK_SHADER_STAGE_MISS_BIT_KHR,
    RayClosestHit      =     VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR,
    RayAnyHit          =     VK_SHADER_STAGE_ANY_HIT_BIT_KHR,
    RayIntersection    =     VK_SHADER_STAGE_INTERSECTION_BIT_KHR,
    RayCallable        =     VK_SHADER_STAGE_CALLABLE_BIT_KHR
};