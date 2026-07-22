//
// Created by Nutellis on 29-Jan-26.
//

#ifndef ALKYONERENDERENGINE_SHADERMANAGER_H
#define ALKYONERENDERENGINE_SHADERMANAGER_H

#include "slang.h"
#include <slang-com-ptr.h>


#include <string>
#include <vector>

#include "rhi/core/shader/RHIShader.h"

struct ContextSlangTargetOptions;




class  ShaderManager
{
public:




private:
    std::vector<RHIShader> shaders;
};


#endif //ALKYONERENDERENGINE_SHADERMANAGER_H