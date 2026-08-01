//
// Created by nutellis on 28/07/2026.
//

#include "utilities/BitmaskAllocator.h"

BitmaskAllocator::BitmaskAllocator(size_t capacity) : capacity(capacity), searchCursor(0) {
    uint32 numOfWords = (capacity + 63) / 64;

    bitmasks.assign(numOfWords, ~0ULL);
    generations.assign(capacity, 1);

    //in case there is a remainder from the division we should force it to 0 and never use it

    uint32 remainingBits = capacity % 64;
    if (remainingBits != 0) {
        uint64 mask = (1ULL << remainingBits) - 1;
        bitmasks.back() &= mask;
    }
}

Handle BitmaskAllocator::Allocate() {
    uint32 numBlocks = static_cast<uint32>(bitmasks.size());

    for (uint32 i = 0; i < numBlocks; i++) {

        uint32 blockIndex = (searchCursor + i) % numBlocks;

        uint64 & block = bitmasks[blockIndex];

        if (block != 0) {
            uint32 bitIndex = std::countr_zero(block);
            block &= ~(1ULL << bitIndex);

            searchCursor = blockIndex;

            uint32 absoluteIndex = (blockIndex * 64) + bitIndex;

            return Handle{ generations[absoluteIndex], absoluteIndex };
        }
    }

    return Handle::Invalid();
}

void BitmaskAllocator::Free(Handle handle) {
    if (!IsValid(handle)) { return; }

    uint32 absoluteIndex = handle.index;
    generations[absoluteIndex]++;

    //wrap back to 0 and restart the generation.
    if (generations[absoluteIndex] == 0) {
        generations[absoluteIndex] = 1;
    }

    uint32 blockIndex = absoluteIndex / 64;
    uint32 bitIndex = absoluteIndex % 64;

    bitmasks[blockIndex] |= (1ULL << bitIndex);
}

bool BitmaskAllocator::IsValid(Handle handle) const {
    if (handle.index >= capacity) return false;

    if (generations[handle.index] != handle.generation) return false;

    //otherwise

    uint32 blockIndex = handle.index / 64;
    uint32 bitIndex = handle.index % 64;

    return (bitmasks[blockIndex] & (1ULL << bitIndex)) == 0;
}


/*
 *
 * BindlessHandle Allocate() {
        // Start searching from the last allocated word to maintain forward momentum.
        // This naturally groups allocations together in memory (Spatial Locality!)
        for (uint32_t i = 0; i < m_freeBits.size(); ++i) {

            uint32_t wordIndex = (m_searchStart + i) % m_freeBits.size();
            uint64_t& word = m_freeBits[wordIndex];

            if (word != 0) { // If there is at least one '1' (free slot) in this 64-bit block

                // Hardware-accelerated scan to find the index of the first '1'
                uint32_t bitIndex = std::countr_zero(word);

                // Flip that specific bit to '0' (Used)
                word &= ~(1ULL << bitIndex);

                // Update our search start so the next allocation is adjacent
                m_searchStart = wordIndex;

                uint32_t absoluteIndex = (wordIndex * 64) + bitIndex;

                return BindlessHandle{ absoluteIndex, m_generations[absoluteIndex] };
            }
        }

        // Out of bindless slots!
        assert(false && "Bindless heap exhausted!");
        return { 0xFFFFFFFF, 0 };
    }

    void Free(BindlessHandle handle) {
        if (!IsValid(handle)) return;

        // Increment generation to invalidate any stale handles holding this ID
        m_generations[handle.index]++;
        if (m_generations[handle.index] == 0) {
            m_generations[handle.index] = 1; // Prevent generation from hitting 0
        }

        // Mark the bit back to '1' (Free)
        uint32_t wordIndex = handle.index / 64;
        uint32_t bitIndex = handle.index % 64;
        m_freeBits[wordIndex] |= (1ULL << bitIndex);
    }

    bool IsValid(BindlessHandle handle) const {
        if (handle.index >= m_capacity) return false;

        // Ensure the generations match
        if (m_generations[handle.index] != handle.generation) return false;

        // Ensure the slot is actually marked as used ('0')
        uint32_t wordIndex = handle.index / 64;
        uint32_t bitIndex = handle.index % 64;
        bool isFree = (m_freeBits[wordIndex] & (1ULL << bitIndex)) != 0;

        return !isFree;
    }

    uint32_t GetCapacity() const { return m_capacity; }
 */
