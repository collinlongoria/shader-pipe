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

#ifdef SHADERPIPE_EXPORTS
    #define SHADERPIPE_API __declspec(dllexport)
#else
    #define SHADERPIPE_API __declspec(dllimport)
#endif

enum class ShaderStage : uint32_t {

};

enum class GlVersion : uint32_t {
    GL_310, // Mobile / Web
    GL_330, // Compatibility
    GL_450, // Desktop
};

SHADERPIPE_API std::string           load_shader_file(const std::string& filename, ShaderStage stage);
SHADERPIPE_API std::vector<uint32_t> glsl_to_spirv   (const std::string& source);
SHADERPIPE_API std::string           spirv_to_glsl   (const std::vector<uint32_t>& source, GlVersion version = GlVersion::GL_450);

#endif //SHADER_PIPE_SHADER_PIPE_HPP