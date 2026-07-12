//
// Created by Nutellis on 08-Mar-26.
//

#ifndef ALKYONERENDERENGINE_BUFFERDESC_H
#define ALKYONERENDERENGINE_BUFFERDESC_H
#include "core/PODTypes.h"

enum class BufferTypeBits : uint32
{
    NONE = 0,
    VERTEX_BUFFER = BIT(0),
    INDEX_BUFFER = BIT(1),
    UNIFORM_BUFFER = BIT(2),
    STORAGE_BUFFER = BIT(3),
    SHADER_DATA_BUFFER = BIT(4),
    TRANSFER_SRC = BIT(5),
    TRANSFER_DST = BIT(6)
};

enum class MemoryUsageStrategy: uint32
{
    GPU_ONLY, // HEAP_TYPE_DEFAULT
    CPU_TO_GPU // HOST_ACCESS_SEQUENTIAL_WRITE_BIT HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT,
};

enum class SharingMode
{
    EXCLUSIVE = 0,
    CONCURRENT
};


struct BufferDesc
{
    uint64 size = 0;
    BufferTypeBits usageFlags = BufferTypeBits::NONE;
    SharingMode sharingMode = SharingMode::EXCLUSIVE;
    MemoryUsageStrategy memoryUsageStrategy = MemoryUsageStrategy::GPU_ONLY;
};


#endif //ALKYONERENDERENGINE_BUFFERDESC_H