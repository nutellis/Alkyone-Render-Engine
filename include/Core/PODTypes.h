//
// Created by Nutellis on 20-Jan-26.
//

#ifndef ALKYONERENDERENGINE_PODTYPES_H
#define ALKYONERENDERENGINE_PODTYPES_H


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
    Handle(uint32 gen, uint32 idx) : generation(gen), index(idx) { }

    uint32 generation: 16;
    uint32 index: 16;

    [[nodiscard]] uint32 ID() const
    {
        return ((generation << 16) | index );
    }
};

using namespace pod;
#endif //ALKYONERENDERENGINE_PODTYPES_H