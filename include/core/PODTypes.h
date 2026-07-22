//
// Created by Nutellis on 20-Jan-26.
//

#ifndef ALKYONERENDERENGINE_PODTYPES_H
#define ALKYONERENDERENGINE_PODTYPES_H

#include <compare>

namespace pod
{
#if defined(_MSC_VER)
    typedef    __int64    int64;

#else
    typedef signed long long int64;
#endif

    typedef int64 ptr;
    typedef unsigned __int64 uint64;

    typedef signed int int32;
    typedef unsigned int uint32;

    typedef unsigned short uint16;
    typedef signed short int16;

    typedef unsigned char uint8;
    typedef uint8 byte;
    typedef char int8;
}

#define BIT(x) (1ULL << (x))


struct Handle
{
    Handle() : generation(0), index(0) { }
    Handle(unsigned int gen, unsigned int index) : generation(gen), index(index) { }

    unsigned int generation: 16;
    unsigned int index: 16;

    [[nodiscard]] unsigned int ID() const
    {
        return ((generation << 16) | index );
    }

    static Handle Invalid() {
        return {0, 0xFFFFFFFF};
    }
    static Handle Create(unsigned int index) {
        return {0, index};
    }

    auto operator<=>(const Handle&) const = default;


};

using namespace pod;
#endif //ALKYONERENDERENGINE_PODTYPES_H