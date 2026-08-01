//
// Created by nutellis on 28/07/2026.
//

#ifndef ALKYONERENDERENGINE_BITMASKALLOCATOR_H
#define ALKYONERENDERENGINE_BITMASKALLOCATOR_H

#include <vector>
#include <bit>

#include "core/PODTypes.h"


class BitmaskAllocator {

   explicit BitmaskAllocator(size_t capacity);
    BitmaskAllocator(BitmaskAllocator const&);
    BitmaskAllocator& operator=(BitmaskAllocator const&);
    BitmaskAllocator(BitmaskAllocator&&);
    BitmaskAllocator& operator=(BitmaskAllocator&&);

    ~BitmaskAllocator();


    Handle Allocate();
    void Free(Handle handle);
    void Reset();
    bool IsValid(Handle handle) const;

private:
    std::vector<uint64> bitmasks;
    std::vector<uint16> generations;
    size_t capacity;
    uint32 searchCursor;
};


#endif //ALKYONERENDERENGINE_BITMASKALLOCATOR_H