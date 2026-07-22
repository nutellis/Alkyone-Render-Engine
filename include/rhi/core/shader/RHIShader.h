//
// Created by nutellis on 17/07/2026.
//

#ifndef ALKYONERENDERENGINE_ISHADER_H
#define ALKYONERENDERENGINE_ISHADER_H

#include <slang.h>
#include <slang-com-ptr.h>

#include <utility>

#include "core/PODTypes.h"

struct RHIShader {

public:
    RHIShader() = default;

    explicit RHIShader(Slang::ComPtr<slang::IComponentType> && program)
    : slangProgram(std::move(program)) {}


    [[nodiscard]] slang::ProgramLayout * GetReflectionLayout() const {
        return slangProgram ? slangProgram->getLayout() : nullptr;
    }

protected:
    Slang::ComPtr<slang::IComponentType> slangProgram;
};


#endif //ALKYONERENDERENGINE_ISHADER_H