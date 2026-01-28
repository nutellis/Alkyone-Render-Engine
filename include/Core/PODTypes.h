//
// Created by Nutellis on 20-Jan-26.
//

#ifndef ALKYONERENDERENGINE_PODTYPES_H
#define ALKYONERENDERENGINE_PODTYPES_H

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

#endif //ALKYONERENDERENGINE_PODTYPES_H