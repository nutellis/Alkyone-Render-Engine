//
// Created by Nutellis on 29-Jan-26.
//

#ifndef ALKYONERENDERENGINE_SHADERMANAGER_H
#define ALKYONERENDERENGINE_SHADERMANAGER_H

#include "slang-com-ptr.h"
#include "slang.h"

struct ContextSlangTargetOptions;

class ShaderManager
{
public:
    /*
    Note
    Currently, the global session type is not thread-safe.
    Applications that wish to compile on multiple threads will need to ensure that each concurrent thread compiles with a distinct global session.

    Note
    Currently, the global session should be freed after any objects created from it.
    */

    Slang::ComPtr<slang::IGlobalSession> globalSession;
    Slang::ComPtr<slang::ISession> slangSession;
    SlangGlobalSessionDesc desc = {};

    bool Initialize(const ContextSlangTargetOptions& slangInitOptions);
};


#endif //ALKYONERENDERENGINE_SHADERMANAGER_H