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

#include "shader_pipe.hpp"

#include <fstream>
#include <string>
#include <sstream>

#include <vulkan/vulkan.h>
#include <glslang/Public/ShaderLang.h>
#include <SPIRV/GlslangToSpv.h>
#include <glslang/Include/ResourceLimits.h>
#include <spirv_cross.hpp>
#include <spirv_glsl.hpp>

static const TBuiltInResource DefaultTBuiltInResource = {
    /* .MaxLights = */ 32,
    /* .MaxClipPlanes = */ 6,
    /* .MaxTextureUnits = */ 32,
    /* .MaxTextureCoords = */ 32,
    /* .MaxVertexAttribs = */ 64,
    /* .MaxVertexUniformComponents = */ 4096,
    /* .MaxVaryingFloats = */ 64,
    /* .MaxVertexTextureImageUnits = */ 32,
    /* .MaxCombinedTextureImageUnits = */ 80,
    /* .MaxTextureImageUnits = */ 32,
    /* .MaxFragmentUniformComponents = */ 4096,
    /* .MaxDrawBuffers = */ 32,
    /* .MaxVertexUniformVectors = */ 128,
    /* .MaxVaryingVectors = */ 8,
    /* .MaxFragmentUniformVectors = */ 16,
    /* .MaxVertexOutputVectors = */ 16,
    /* .MaxFragmentInputVectors = */ 15,
    /* .MinProgramTexelOffset = */ -8,
    /* .MaxProgramTexelOffset = */ 7,
    /* .MaxClipDistances = */ 8,
    /* .MaxComputeWorkGroupCountX = */ 65535,
    /* .MaxComputeWorkGroupCountY = */ 65535,
    /* .MaxComputeWorkGroupCountZ = */ 65535,
    /* .MaxComputeWorkGroupSizeX = */ 1024,
    /* .MaxComputeWorkGroupSizeY = */ 1024,
    /* .MaxComputeWorkGroupSizeZ = */ 64,
    /* .MaxComputeUniformComponents = */ 1024,
    /* .MaxComputeTextureImageUnits = */ 16,
    /* .MaxComputeImageUniforms = */ 8,
    /* .MaxComputeAtomicCounters = */ 8,
    /* .MaxComputeAtomicCounterBuffers = */ 1,
    /* .MaxVaryingComponents = */ 60,
    /* .MaxVertexOutputComponents = */ 64,
    /* .MaxGeometryInputComponents = */ 64,
    /* .MaxGeometryOutputComponents = */ 128,
    /* .MaxFragmentInputComponents = */ 128,
    /* .MaxImageUnits = */ 8,
    /* .MaxCombinedImageUnitsAndFragmentOutputs = */ 8,
    /* .MaxCombinedShaderOutputResources = */ 8,
    /* .MaxImageSamples = */ 0,
    /* .MaxVertexImageUniforms = */ 0,
    /* .MaxTessControlImageUniforms = */ 0,
    /* .MaxTessEvaluationImageUniforms = */ 0,
    /* .MaxGeometryImageUniforms = */ 0,
    /* .MaxFragmentImageUniforms = */ 8,
    /* .MaxCombinedImageUniforms = */ 8,
    /* .MaxGeometryTextureImageUnits = */ 16,
    /* .MaxGeometryOutputVertices = */ 256,
    /* .MaxGeometryTotalOutputComponents = */ 1024,
    /* .MaxGeometryUniformComponents = */ 1024,
    /* .MaxGeometryVaryingComponents = */ 64,
    /* .MaxTessControlInputComponents = */ 128,
    /* .MaxTessControlOutputComponents = */ 128,
    /* .MaxTessControlTextureImageUnits = */ 16,
    /* .MaxTessControlUniformComponents = */ 1024,
    /* .MaxTessControlTotalOutputComponents = */ 4096,
    /* .MaxTessEvaluationInputComponents = */ 128,
    /* .MaxTessEvaluationOutputComponents = */ 128,
    /* .MaxTessEvaluationTextureImageUnits = */ 16,
    /* .MaxTessEvaluationUniformComponents = */ 1024,
    /* .MaxTessPatchComponents = */ 120,
    /* .MaxPatchVertices = */ 32,
    /* .MaxTessGenLevel = */ 64,
    /* .MaxViewports = */ 16,
    /* .MaxVertexAtomicCounters = */ 0,
    /* .MaxTessControlAtomicCounters = */ 0,
    /* .MaxTessEvaluationAtomicCounters = */ 0,
    /* .MaxGeometryAtomicCounters = */ 0,
    /* .MaxFragmentAtomicCounters = */ 8,
    /* .MaxCombinedAtomicCounters = */ 8,
    /* .MaxAtomicCounterBindings = */ 1,
    /* .MaxVertexAtomicCounterBuffers = */ 0,
    /* .MaxTessControlAtomicCounterBuffers = */ 0,
    /* .MaxTessEvaluationAtomicCounterBuffers = */ 0,
    /* .MaxGeometryAtomicCounterBuffers = */ 0,
    /* .MaxFragmentAtomicCounterBuffers = */ 1,
    /* .MaxCombinedAtomicCounterBuffers = */ 1,
    /* .MaxAtomicCounterBufferSize = */ 16384,
    /* .MaxTransformFeedbackBuffers = */ 4,
    /* .MaxTransformFeedbackInterleavedComponents = */ 64,
    /* .MaxCullDistances = */ 8,
    /* .MaxCombinedClipAndCullDistances = */ 8,
    /* .MaxSamples = */ 4,
    /* .maxMeshOutputVerticesNV = */ 256,
    /* .maxMeshOutputPrimitivesNV = */ 512,
    /* .maxMeshWorkGroupSizeX_NV = */ 32,
    /* .maxMeshWorkGroupSizeY_NV = */ 1,
    /* .maxMeshWorkGroupSizeZ_NV = */ 1,
    /* .maxTaskWorkGroupSizeX_NV = */ 32,
    /* .maxTaskWorkGroupSizeY_NV = */ 1,
    /* .maxTaskWorkGroupSizeZ_NV = */ 1,
    /* .maxMeshViewCountNV = */ 4,
    /* .maxMeshOutputVerticesEXT = */ 256,
    /* .maxMeshOutputPrimitivesEXT = */ 256,
    /* .maxMeshWorkGroupSizeX_EXT = */ 128,
    /* .maxMeshWorkGroupSizeY_EXT = */ 128,
    /* .maxMeshWorkGroupSizeZ_EXT = */ 128,
    /* .maxTaskWorkGroupSizeX_EXT = */ 128,
    /* .maxTaskWorkGroupSizeY_EXT = */ 128,
    /* .maxTaskWorkGroupSizeZ_EXT = */ 128,
    /* .maxMeshViewCountEXT = */ 4,
    /* .maxDualSourceDrawBuffersEXT = */ 1,

    /* .limits = */ {
        /* .nonInductiveForLoops = */ 1,
        /* .whileLoops = */ 1,
        /* .doWhileLoops = */ 1,
        /* .generalUniformIndexing = */ 1,
        /* .generalAttributeMatrixVectorIndexing = */ 1,
        /* .generalVaryingIndexing = */ 1,
        /* .generalSamplerIndexing = */ 1,
        /* .generalVariableIndexing = */ 1,
        /* .generalConstantMatrixVectorIndexing = */ 1,
    }};

namespace shaderpipe {

struct DescriptorBindingInfo {
    uint32_t set;
    uint32_t binding;
    std::string name;
    VkDescriptorType type;
    uint32_t count;
    VkShaderStageFlags stageFlags;
};

struct PushConstantInfo {
    uint32_t offset;
    uint32_t size;
    VkShaderStageFlags stageFlags;
};

struct InputAttributeInfo {
    uint32_t location;
    std::string name;
    uint32_t vecSize;
    uint32_t bitWidth;
};

struct ShaderReflection {
    std::vector<DescriptorBindingInfo> descriptorBindings;
    std::vector<PushConstantInfo> pushConstants;
    std::vector<InputAttributeInfo> inputs;
    std::vector<InputAttributeInfo> outputs;
};

struct CompiledShader {
    std::vector<uint32_t> spirv;
    ShaderReflection reflection;
};

static EShLanguage shader_stage_to_glslang (ShaderStage s) {
    switch (s) {
    default: case ShaderStage::VERTEX: return EShLangVertex;
    case ShaderStage::TESS_CONTROL: return EShLangTessControl;
    case ShaderStage::TESS_EVAL: return EShLangTessEvaluation;
    case ShaderStage::GEOMETRY: return EShLangGeometry;
    case ShaderStage::FRAGMENT: return EShLangFragment;
    case ShaderStage::COMPUTE: return EShLangCompute;
    case ShaderStage::RAYGEN: return EShLangRayGen;
    case ShaderStage::INTERSECT: return EShLangIntersect;
    case ShaderStage::ANY_HIT: return EShLangAnyHit;
    case ShaderStage::CLOSEST_HIT: return EShLangClosestHit;
    case ShaderStage::MISS: return EShLangMiss;
    case ShaderStage::CALLABLE: return EShLangCallable;
    case ShaderStage::TASK: return EShLangTask;
    case ShaderStage::MESH: return EShLangMesh;
    }  // not sure what else to default to
};

static uint32_t gl_version_enum_to_int (GlVersion v) {
    switch (v) {
    case GlVersion::GL_310: return 310;
    case GlVersion::GL_330: return 330;
    default: case GlVersion::GL_450: return 450;
        // Maybe default should be 310
    }
};

static glslang::EShTargetClientVersion vk_version_to_glslang(VKVersion v) {
    switch (v) {
    case VKVersion::VK_1_0: return glslang::EShTargetVulkan_1_0;
    case VKVersion::VK_1_1: return glslang::EShTargetVulkan_1_1;
    case VKVersion::VK_1_2: return glslang::EShTargetVulkan_1_2;
    case VKVersion::VK_1_3: return glslang::EShTargetVulkan_1_3;
    case VKVersion::VK_1_4: return glslang::EShTargetVulkan_1_4;
    }
    return glslang::EShTargetVulkan_1_0;
}

static glslang::EShTargetLanguageVersion vk_version_to_spirv_version(VKVersion v) {
    switch (v) {
    case VKVersion::VK_1_0: return glslang::EShTargetSpv_1_0;
    case VKVersion::VK_1_1: return glslang::EShTargetSpv_1_3;
    case VKVersion::VK_1_2: return glslang::EShTargetSpv_1_5;
    case VKVersion::VK_1_3:
    case VKVersion::VK_1_4:
        return glslang::EShTargetSpv_1_6; // TODO: may need to handle compatability for spv 1.5
    }
    return glslang::EShTargetSpv_1_0;
}

uint32_t get_glsl_version(const std::string& source) {
    static std::string substr = "#version";

    std::size_t pos = source.find(substr);
    if (pos != std::string::npos) {
        std::size_t end = source.find('\n', pos);
        std::string line = source.substr(pos, end - pos);

        std::istringstream iss(line);
        std::string directive, versionStr, profile;
        iss >> directive >> versionStr >> profile; // profile can be safely ignored for now.

        if (directive == "#version") {
            int version = std::stoi(versionStr);
            return version;
        }
    }
    return 0;
}

std::string load_shader_file(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Shader file could not be opened.");
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    file.close();

    return buffer.str();
}

std::vector<uint32_t> glsl_to_spirv(const std::string &source, ShaderStage stage, VKVersion targetVulkanVersion) {
    const char* glslSource = source.c_str();

    glslang::InitializeProcess();

    auto sStage = shader_stage_to_glslang(stage);
    glslang::TShader shader(sStage);
    shader.setStrings(&glslSource, 1);

    uint32_t glslVersion = get_glsl_version(source);
    const auto vulkanVersion = vk_version_to_glslang(targetVulkanVersion);
    const auto spvVersion = vk_version_to_spirv_version(targetVulkanVersion);

    shader.setEnvInput(glslang::EShSourceGlsl, sStage, glslang::EShClientVulkan, static_cast<int>(glslVersion));
    shader.setEnvClient(glslang::EShClientVulkan, vulkanVersion);
    shader.setEnvTarget(glslang::EShTargetSpv, spvVersion);

    if (!shader.parse(&DefaultTBuiltInResource, 450, false, EShMsgDefault)) {
        throw std::runtime_error(shader.getInfoLog());
    }

    glslang::TProgram program;
    program.addShader(&shader);

    if (!program.link(EShMsgDefault)) {
        throw std::runtime_error(program.getInfoLog());
    }

    std::vector<uint32_t> spirv;
    glslang::GlslangToSpv(*program.getIntermediate(sStage), spirv);

    glslang::FinalizeProcess();

    return spirv;
}

CompiledShader glsl_to_spirv_with_reflection(const std::string &source, ShaderStage stage) noexcept {
    auto spirv = glsl_to_spirv(source, stage);
    auto refl = reflect_spirv(spirv);
    return { std::move(spirv), std::move(refl) };
}

std::string spirv_to_glsl (const std::vector<uint32_t>& source, GlVersion version) {
    spirv_cross::CompilerGLSL compiler(source);

    // Options for GLSL output
    spirv_cross::CompilerGLSL::Options options;
    options.version = gl_version_enum_to_int(version);
    options.es = false;
    options.vulkan_semantics = false;
    options.separate_shader_objects = false;
    options.enable_420pack_extension = (version != GlVersion::GL_450); // TODO: If GL versions are extended, this line will need to be altered to properly handle more than one version that doesn't need the extension.

    compiler.set_common_options(options);

    return compiler.compile();
}
}