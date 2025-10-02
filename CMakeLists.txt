cmake_minimum_required(VERSION 3.20)

project(ShaderPipe
        VERSION 1.0
        LANGUAGES CXX)

# Output directory
set(OUT_BIN ${CMAKE_SOURCE_DIR}/build/$<CONFIG>/bin)
set(OUT_LIB ${CMAKE_SOURCE_DIR}/build/$<CONFIG>/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUT_BIN})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUT_LIB})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUT_LIB})

# Third party
add_subdirectory(external/SPIRV-Headers)
add_subdirectory(external/SPIRV-Tools)
add_subdirectory(external/glslang)
add_subdirectory(external/SPIRV-Cross)

set(SPIRV-Headers_SOURCE_DIR "${CMAKE_SOURCE_DIR}/external/SPIRV-Headers")

add_library(shader_pipe SHARED
        src/shader_pipe.cpp
        include/shader_pipe.hpp
)

target_compile_definitions(shader_pipe PRIVATE SHADERPIPE_EXPORTS)

target_include_directories(shader_pipe PUBLIC
        include
        external/glslang
        external/SPIRV-Tools/include
        external/SPIRV-Cross
)

target_link_libraries(shader_pipe PRIVATE
        glslang
        SPIRV
        SPIRV-Tools-opt
        spirv-cross-core
        spirv-cross-glsl
)