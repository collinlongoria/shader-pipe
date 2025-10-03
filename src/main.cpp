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
#include <fstream>

void saveSpirvToFile(const std::vector<uint32_t>& spirv, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary | std::ios::out);
    if (!file) {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }

    file.write(reinterpret_cast<const char*>(spirv.data()),
               spirv.size() * sizeof(uint32_t));
}

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

        // Compile GLSL → SPIR-V
        std::vector<uint32_t> spirv = glsl_to_spirv(vsSource, ShaderStage::VERTEX);
        std::cout << "Compiled SPIR-V size: " << spirv.size() << " words\n";
        saveSpirvToFile(spirv, "test.spv");

        for (int i = 0; i < 5 && i < spirv.size(); i++)
            std::cout << std::hex << spirv[i] << " ";
        std::cout << "\n";

        // Recompile SPIR-V → GLSL
        std::string glsl330 = spirv_to_glsl(spirv, GlVersion::GL_330);
        std::cout << "\n[Recompiled GLSL 330]\n" << glsl330 << "\n";

        std::string glsl450 = spirv_to_glsl(spirv, GlVersion::GL_450);
        std::cout << "\n[Recompiled GLSL 450]\n" << glsl450 << "\n";

    } catch (const std::exception& e) {
        std::cerr << "Shader test failed: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
