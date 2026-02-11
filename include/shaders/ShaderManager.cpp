//
// Created by Nutellis on 29-Jan-26.
//

#include <core/PODTypes.h>
#include <array>
#include <shaders/ShaderManager.h>

#include "rhi/IGraphicsContext.h"
#include "spdlog/spdlog.h"

bool ShaderManager::Initialize(const ContextSlangTargetOptions& slangInitOptions)
{
    /*
     If you want to enable GLSL compatibility mode, you need to set SlangGlobalSessionDesc::enableGLSL to true when calling createGlobalSession().
     This will load the necessary GLSL intrinsic module for compiling GLSL code.
     Without this setting, compiling GLSL code will result in an error.
     */
    createGlobalSession(&desc, globalSession.writeRef());

    // create session
    auto slangTargets{
        std::to_array<slang::TargetDesc>({ {
            .format = slangInitOptions.format,
            .profile = globalSession->findProfile(slangInitOptions.profile.c_str()),
            }
        })
    };

    auto slangOptions{ std::to_array<slang::CompilerOptionEntry>({ {
        slangInitOptions.name,
        slangInitOptions.value
    } }) };

    const char * searchPaths[] = { "../shaders/" };

    slang::SessionDesc slangSessionDesc{
        .targets = slangTargets.data(),
        .targetCount = static_cast<SlangInt>(slangTargets.size()),
        .defaultMatrixLayoutMode = SLANG_MATRIX_LAYOUT_COLUMN_MAJOR,
        .searchPaths = searchPaths,
        .searchPathCount = 1,
        .compilerOptionEntries = slangOptions.data(),
        .compilerOptionEntryCount = static_cast<uint32>(slangOptions.size())

    };

    if (globalSession->createSession(slangSessionDesc, slangSession.writeRef()) != SLANG_OK)
    {
        spdlog::error("Slang failed to Create Session");
        return false;
    }

    spdlog::info("Slang Session Created");

    return true;
}


/*bool vkutil::load_shader_module(const char* filePath,
    VkDevice device,
    VkShaderModule* outShaderModule)
{
    // open the file. With cursor at the end
    std::ifstream file(filePath, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        return false;
    }

    // find what the size of the file is by looking up the location of the cursor
    // because the cursor is at the end, it gives the size directly in bytes
    size_t fileSize = (size_t)file.tellg();

    // spirv expects the buffer to be on uint32, so make sure to reserve a int
    // vector big enough for the entire file
    std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));

    // put file cursor at beginning
    file.seekg(0);

    // load the entire file into the buffer
    file.read((char*)buffer.data(), fileSize);

    // now that the file is loaded into the buffer, we can close it
    file.close();

    // create a new shader module, using the buffer we loaded
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.pNext = nullptr;

    // codeSize has to be in bytes, so multply the ints in the buffer by size of
    // int to know the real size of the buffer
    createInfo.codeSize = buffer.size() * sizeof(uint32_t);
    createInfo.pCode = buffer.data();

    // check that the creation goes well.
    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        return false;
    }
    *outShaderModule = shaderModule;
    return true;
}*/