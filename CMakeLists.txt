cmake_minimum_required(VERSION 3.20)

project(ShaderPipe
        VERSION 1.0
        LANGUAGES CXX)

# Output directory
set(OUT_BIN ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/bin)
set(OUT_LIB ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUT_BIN})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUT_LIB})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUT_LIB})

# Third party
set(SPIRV_SKIP_EXECUTABLES ON CACHE BOOL "" FORCE)
set(SPIRV_SKIP_TESTS ON CACHE BOOL "" FORCE)
set(ENABLE_GLSLANG_BINARIES OFF CACHE BOOL "" FORCE)
set(SPIRV_TOOLS_BUILD_CPP_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(external/SPIRV-Headers)
add_subdirectory(external/SPIRV-Tools)
add_subdirectory(external/glslang)
add_subdirectory(external/SPIRV-Cross)

add_library(shaderpipe SHARED
        src/shader_pipe.cpp
        include/shader_pipe.hpp
)

target_compile_definitions(shaderpipe PRIVATE SHADERPIPE_EXPORTS)

target_include_directories(shaderpipe PUBLIC
        include
        external/glslang
        external/SPIRV-Tools/include
        external/SPIRV-Cross
)

target_link_libraries(shaderpipe PRIVATE
        glslang
        SPIRV
        SPIRV-Tools-opt
        spirv-cross-core
        spirv-cross-glsl
)

# removes "lib" prefix
set_target_properties(shaderpipe PROPERTIES
        OUTPUT_NAME "shaderpipe"
        PREFIX ""
)

# Test executable
add_executable(shaderpipe_test src/main.cpp)

target_link_libraries(shaderpipe_test
        PRIVATE shaderpipe
)