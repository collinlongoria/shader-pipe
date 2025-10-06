/*
* File: main
* Project: ShaderPipe
* Author: colli
* Created on: 10/2/2025
*
* Copyright (c) 2025 Collin Longoria
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/

#include "shader_pipe.hpp"
#include <iostream>

int main() {
    try {
        // Load a trivial GLSL vertex shader (you can also use load_shader_file())
        const std::string vsSource = R"(
            #version 330 core
            layout(location = 0) in vec3 aPos;
            void main() {
                gl_Position = vec4(aPos, 1.0);
            }
        )";

        // Get version of source shader
        uint32_t version = get_glsl_version(vsSource);
        std::cout << "Original shader version: " << version << "\n";

        // Compile GLSL → SPIR-V
        std::vector<uint32_t> spirv = glsl_to_spirv(vsSource, ShaderStage::VERTEX);
        std::cout << "Compiled SPIR-V size: " << spirv.size() << " words\n";

        // Recompile SPIR-V → GLSL
        std::string glsl330 = spirv_to_glsl(spirv, GlVersion::GL_330);
        std::cout << "\n[Recompiled GLSL 330]\n" << glsl330 << "\n";

        std::string glsl450 = spirv_to_glsl(spirv, GlVersion::GL_450);
        std::cout << "\n[Recompiled GLSL 450]\n" << glsl450 << "\n";

        // Get final version of shader
        uint32_t version2 = get_glsl_version(glsl450);
        std::cout << "Updated shader version: " << version2 << "\n";

    } catch (const std::exception& e) {
        std::cerr << "Shader test failed: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
