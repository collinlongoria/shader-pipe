/*
* File: shader_pipe
* Project: shader-pipe
* Author: Collin Longoria
* Created on: 10/2/2025
*
* Copyright (c) 2025 Collin Longoria
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/

#ifndef SHADER_PIPE_SHADER_PIPE_HPP
#define SHADER_PIPE_SHADER_PIPE_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

#ifdef SHADERPIPE_EXPORTS
    #define SHADERPIPE_API __declspec(dllexport)
#else
    #define SHADERPIPE_API __declspec(dllimport)
#endif

namespace shaderpipe {
enum class ShaderStage : uint32_t {
    VERTEX,
    TESS_CONTROL,
    TESS_EVAL,
    GEOMETRY,
    FRAGMENT,
    COMPUTE,
    RAYGEN,
    RAYGEN_NV = RAYGEN,
    INTERSECT,
    INTERSECT_NV = INTERSECT,
    ANY_HIT,
    ANY_HIT_NV = ANY_HIT,
    CLOSEST_HIT,
    CLOSEST_HIT_NV = CLOSEST_HIT,
    MISS,
    MISS_NV = MISS,
    CALLABLE,
    CALLABLE_NV = CALLABLE,
    TASK,
    TASK_NV = TASK,
    MESH,
    MESH_NV = MESH,
}; // Same ones as provided in glslang

enum class GlVersion : uint32_t {
    GL_310, // Mobile / Web
    GL_330, // Compatibility
    GL_450, // Desktop
};

// This enum will also determine the spir-v compiler version.
enum class VKVersion : uint32_t {
    VK_1_0,
    VK_1_1,
    VK_1_2,
    VK_1_3,
    VK_1_4,
};

struct SHADERPIPE_API DescriptorBindingInfo {
    uint32_t set;
    uint32_t binding;
    std::string name;
    VkDescriptorType type;
    uint32_t count;
    VkShaderStageFlags stageFlags;
};

struct SHADERPIPE_API PushConstantInfo {
    uint32_t offset;
    uint32_t size;
    VkShaderStageFlags stageFlags;
};

struct SHADERPIPE_API InputAttributeInfo {
    uint32_t location;
    std::string name;
    uint32_t vecSize;
    uint32_t bitWidth;
};

struct SHADERPIPE_API ShaderReflection {
    std::vector<DescriptorBindingInfo> descriptorBindings;
    std::vector<PushConstantInfo> pushConstants;
    std::vector<InputAttributeInfo> inputs;
    std::vector<InputAttributeInfo> outputs;
};

struct SHADERPIPE_API CompiledShader {
    std::vector<uint32_t> spirv;
    ShaderReflection reflection;
};

SHADERPIPE_API uint32_t              get_glsl_version              (const std::string& source);
SHADERPIPE_API std::string           load_shader_file              (const std::string& filename);
SHADERPIPE_API std::vector<uint32_t> glsl_to_spirv                 (const std::string& source, ShaderStage stage, VKVersion targetVulkanVersion);
SHADERPIPE_API ShaderReflection      reflect_spirv                 (const std::vector<uint32_t>& source);
SHADERPIPE_API CompiledShader      glsl_to_spirv_with_reflection   (const std::string& source, ShaderStage stage, VKVersion targetVulkanVersion) noexcept;
SHADERPIPE_API std::string           spirv_to_glsl                 (const std::vector<uint32_t>& source, GlVersion version = GlVersion::GL_450);
}

#endif //SHADER_PIPE_SHADER_PIPE_HPP