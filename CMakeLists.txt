cmake_minimum_required(VERSION 4.1)
project(AlkyoneRenderEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# force x64
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "Alkyone must be built as a 64-bit application!")
endif()


set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

set(VMA_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(DGLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(DBUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(KTX_FEATURE_TOOLS OFF CACHE BOOL "" FORCE)
set(KTX_FEATURE_TESTS OFF CACHE BOOL "" FORCE)
set(KTX_FEATURE_DOC OFF CACHE BOOL "" FORCE)

set(SLANG_ENABLE_TESTS OFF CACHE BOOL "" FORCE)
set(SLANG_ENABLE_EXAMPLES OFF CACHE BOOL "" FORCE)

set(TINYGLTF_HEADER_ONLY ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL OFF CACHE INTERNAL "" FORCE)

# Slang Fetch -----------------------------------------------------------
#include(FetchContent)
#
#FetchContent_Declare(
#        slang
#        GIT_REPOSITORY https://github.com/shader-slang/slang.git
#        GIT_TAG 97c5d06643364a31a94d3358b7b8788defad5ea2
#        GIT_SHALLOW ON
#)
#FetchContent_MakeAvailable(slang)


#-------------------------------------------------------------------------



add_subdirectory(external/glfw-3.4)
add_subdirectory(external/volk-1.4.304)
add_subdirectory(external/VulkanMemoryAllocator-3.3.0)
add_subdirectory(external/glm-1.0.3)
add_subdirectory(external/tinyobjloader-1.0.6)
add_subdirectory(external/KTX-Software-4.4.2)
add_subdirectory(external/spdlog-1.17.0)
add_subdirectory(external/tinygltf-2.9.7)

find_package(Vulkan REQUIRED)

include_directories($ENV{VULKAN_SDK}/include/slang)
find_library(Slang_LIBRARY NAMES slang slang-compiler HINTS "$ENV{VULKAN_SDK}/lib" REQUIRED)

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(AlkyoneRenderer main.cpp ${SRC_FILES}
        include/rhi/core/shader/ShaderCursor.h
        include/rhi/core/shader/ShaderCursor.cpp
        include/rhi/core/shader/IShaderObject.h
        include/rhi/core/shader/RHIShader.h
        include/rhi/vulkan/VulkanShader.h
        include/rhi/descriptors/ShaderCompileDesc.h
        include/rhi/vulkan/VulkanBufferTransforms.h
        include/resources/ResourceHandle.h)

target_compile_options(AlkyoneRenderer
        PRIVATE
        $<$<CONFIG:Debug>:/W3 /sdl /Od>
)

target_compile_options(AlkyoneRenderer
        PRIVATE
        $<$<CONFIG:Release>:/W3 /O2 /sdl>
)

target_include_directories(AlkyoneRenderer PRIVATE
        include
        src
)

target_compile_definitions(AlkyoneRenderer PRIVATE VK_NO_PROTOTYPES)
if (WIN32)
    target_compile_definitions(AlkyoneRenderer PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

target_link_libraries(AlkyoneRenderer PRIVATE
        glfw
        volk
        GPUOpen::VulkanMemoryAllocator
        glm::glm
        tinyobjloader
        ktx
        ${Slang_LIBRARY}
        ${SlangCompiler_LIBRARY}
        spdlog::spdlog
        Vulkan::Vulkan
        tinygltf
)

#function (add_slang_shader_target TARGET)
#    cmake_parse_arguments ("SHADER" "" "" "SOURCES" ${ARGN})
#    set (SHADERS_DIR ${CMAKE_CURRENT_LIST_DIR}/shaders)
#    set (ENTRY_POINTS -entry vertMain -entry fragMain)
#    add_custom_command (
#            OUTPUT ${SHADERS_DIR}
#            COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADERS_DIR}
#    )
#    add_custom_command (
#            OUTPUT  ${SHADERS_DIR}/slang.spv
#            COMMAND ${SLANGC_EXECUTABLE} ${SHADER_SOURCES} -target spirv -profile spirv_1_4 -emit-spirv-directly -fvk-use-entrypoint-name ${ENTRY_POINTS} -o slang.spv
#            WORKING_DIRECTORY ${SHADERS_DIR}
#            DEPENDS ${SHADERS_DIR} ${SHADER_SOURCES}
#            COMMENT "Compiling Slang Shaders"
#            VERBATIM
#    )
#    add_custom_target (${TARGET} DEPENDS ${SHADERS_DIR}/slang.spv)
#endfunction()
#
#add_slang_shader_target( foo SOURCES ${SHADER_SLANG_SOURCES})
#add_dependencies(bar foo)