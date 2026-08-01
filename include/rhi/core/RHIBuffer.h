//
// Created by Nutellis on 08-Mar-26.
//

#ifndef ALKYONERENDERENGINE_IBUFFER_H
#define ALKYONERENDERENGINE_IBUFFER_H
#include "core/PODTypes.h"

struct BufferHandle {
    Handle handle;
};

struct RHIBuffer {

    std::unordered_map<std::string, size_t> offsets;
};


#endif //ALKYONERENDERENGINE_IBUFFER_H