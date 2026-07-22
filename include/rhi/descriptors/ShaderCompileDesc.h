//
// Created by nutellis on 17/07/2026.
//

#ifndef ALKYONERENDERENGINE_SHADERCOMPILEDESC_H
#define ALKYONERENDERENGINE_SHADERCOMPILEDESC_H
#include <string>


struct ShaderCompileDesc
{
    std::string filename;
    std::string vertexEntryPoint = "vertexMain";
    std::string fragmentEntryPoint = "fragmentMain";
};

#endif //ALKYONERENDERENGINE_SHADERCOMPILEDESC_H