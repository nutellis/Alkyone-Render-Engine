//
// Created by Nutellis on 25-Jan-26.
//

#ifndef ALKYONERENDERENGINE_SLOTMAP_H
#define ALKYONERENDERENGINE_SLOTMAP_H

#include "Containers\Array.h"

#include

template <typename T>
struct SlotMap
{

public:
    struct ElementId
    {
        ElementId() : isAlive(0), generation(0), index(0)
        {
        }

        uint32 isAlive : 1;
        uint32 generation: 15;
        uint32 index: 16;

        [[nodiscard]] uint32 ID() const
        {
            return ( (isAlive << 31) | (generation << 16) | index );
        }
    };

public:
    ElementId createSlot(T&& slotObject = {})
    {
        //that means that we have empty slots somewhere
        if (freeSlotsTracker.Size() > 0)
        {
            //get the end of the freeSlotsTracker
            size_t const index = static_cast<uint32>(freeSlotsTracker.Back());
            //remove it from the freeSlotsTracker
            freeSlotsTracker.PopBack();

            data[index] = std::move(slotObject);

            indices[index].isAlive = 1;

            return indices[index];
        }
        else //first time using this slot
        {
            uint32 const index = static_cast<uint32>(data.Size());
            data.PushBack(std::move(slotObject));

            ElementId id = ElementId(1,1,index);
            indices.PushBack(id);
            return id;
        }
    }

    bool destroySlot(ElementId id)
    {
        //check if the id is valid
        if (IsIdValid(id))
        {
            size_t const index = static_cast<size_t>(id.index);

            //free the data at the index
            delete data[index];
            data[index] = T{};

            indices[index].generation = (indices[index].generation + 1) & 0x7FFF;

            indices[index].isAlive = 0;

            if (indices[static_cast<size_t>(index)] < std::numeric_limits<uint16>::max())
            {
                freeSlotsTracker.PushBack(index);
            }
            return true;
        }
        return false;
    }

    bool IsIdValid(ElementId id)
    {
        size_t const index = static_cast<uint32>(id.index);
        const bool validation =  index < data.Size()
            && indices[index].isAlive
            && id.generation == indices[index].generation;

        return validation;

    }

private:
    TArray<ElementId> indices;
    TArray<T> data;

    // keeps a LIFO queue of the empty indices on data.
    TArray<size_t> freeSlotsTracker;

};


#endif //ALKYONERENDERENGINE_SLOTMAP_H