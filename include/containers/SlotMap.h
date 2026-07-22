//
// Created by Nutellis on 25-Jan-26.
//

#ifndef ALKYONERENDERENGINE_SLOTMAP_H
#define ALKYONERENDERENGINE_SLOTMAP_H

#include <optional>
#include <core/PODTypes.h>
#include <vector>

template <typename T>
struct SlotMap
{

public:
    Handle createSlot(T&& slotObject)
    {
        //that means that we have empty slots somewhere
        if (freeSlotsTracker.empty() == false)
        {
            //get the end of the freeSlotsTracker
            size_t const index = freeSlotsTracker.back();
            //remove it from the freeSlotsTracker
            freeSlotsTracker.pop_back();

            data[index] = std::move(slotObject);

            return indices[index];
        }

            //first time using this slot
            size_t const index = data.size();
            data.push_back(std::move(slotObject));

            Handle id = Handle(1,static_cast<uint32>(index));
            indices.push_back(id);
            return id;

    }

    bool destroySlot(Handle id)
    {
        //check if the id is valid
        if (IsIdValid(id))
        {
            size_t const index = static_cast<size_t>(id.index);

            //free the data at the index
            //delete data[index];
            data[index].reset();

            indices[index].generation = (indices[index].generation + 1) & 0xFFFF;

            if (indices[static_cast<size_t>(index)].generation < std::numeric_limits<uint16>::max())
            {
                freeSlotsTracker.push_back(index);
            }
            return true;
        }
        return false;
    }

    bool IsIdValid(Handle id)
    {
        size_t const index = static_cast<uint32>(id.index);
        const bool validation =  index < data.size()
            && id.generation == indices[index].generation;

        return validation;
    }

    T& operator[](Handle id)
    {
        return data[static_cast<size_t>(id.index)].value();
    }

    T* GetSlot(Handle id) {
        if (IsIdValid(id)) {
            return &data[static_cast<size_t>(id.index)].value();
        }
        return nullptr;
    }

    void Clear() {
        data.clear();
        indices.clear();
        freeSlotsTracker.clear();
    }

private:
    std::vector<Handle> indices;
    std::vector<std::optional<T>> data;

    // keeps a LIFO queue of the empty indices on data.
    std::vector<size_t> freeSlotsTracker;

};


#endif //ALKYONERENDERENGINE_SLOTMAP_H