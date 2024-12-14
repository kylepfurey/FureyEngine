// .h
// Dynamic Pointer Array Script
// by Kyle Furey

#pragma once
#include "../Standard/Standard.h"

namespace FureyEngine {
    /** Stores a dynamic array of pointers. */
    template<typename Type>
    class PointerArray final {
        // ARRAY

        /** The underlying vector of shared pointers. */
        std::vector<std::shared_ptr<Type> > Array;

    public:
        // ARRAY

        /** Adds new data to the array. */
        Type &Add() {
            return *Array.emplace_back(std::make_shared<Type>()).get();
        }

        /** Adds new data to the array. */
        template<typename... ArgumentTypes>
        Type &Add(ArgumentTypes... Arguments) {
            return *Array.emplace_back(std::make_shared<Type>(Arguments...)).get();
        }

        /** Inserts new data in the array at the given index. */
        Type &Insert(const int &Index) {
            if (!InRange(Index)) {
                throw std::runtime_error("ERROR: Pointer array index is out of range!");
            }
            return *Array.emplace(Array.begin() + Index, std::make_shared<Type>()).get();
        }

        /** Inserts new data in the array at the given index. */
        template<typename... ArgumentTypes>
        Type &Insert(const int &Index,
                     ArgumentTypes... Arguments) {
            if (!InRange(Index)) {
                throw std::runtime_error("ERROR: Pointer array index is out of range!");
            }
            return *Array.emplace(Array.begin() + Index, std::make_shared<Type>(Arguments...)).get();
        }

        /** Removes the data at the given index from the array. */
        void Remove(const int &Index) {
            if (!InRange(Index)) {
                throw std::runtime_error("ERROR: Pointer array index is out of range!");
            }
            Array.erase(Array.begin() + Index);
        }

        /** Clears this array. */
        int Clear() {
            const int Count = static_cast<int>(Array.size());
            Array.clear();
            return Count;
        }

        /** Swaps the data at the given indices. */
        void Swap(const int &IndexA, const int &IndexB) {
            if (!InRange(IndexA) || !InRange(IndexB)) {
                throw std::runtime_error("ERROR: Pointer array index is out of range!");
            }
            const auto Swapped = Array[IndexA];
            Array[IndexA] = Array[IndexB];
            Array[IndexB] = Swapped;
        }

        /** Returns if the given index is in the range of this array. */
        [[nodiscard]] bool InRange(const int &Index) const {
            return Index >= 0 && Index < Array.size();
        }

        /** Returns the given data's index in this array or -1 if it is not present. */
        [[nodiscard]] int Find(const Type &Data) const {
            for (int i = 0; i < Array.size(); ++i) {
                if (Array[i].get() == &Data) {
                    return i;
                }
            }
            return -1;
        }

        /** Returns whether the given data is in this array. */
        [[nodiscard]] bool Contains(const Type &Data) const {
            return Find(Data) != -1;
        }

        /** Returns the total number of data in this array. */
        [[nodiscard]] int Count() const {
            return static_cast<int>(Array.size());
        }

        /** Returns whether this array is empty. */
        [[nodiscard]] bool IsEmpty() const {
            return Array.empty();
        }

        /** Returns the data at the given index. */
        [[nodiscard]] Type &Get(const int &Index) const {
            if (!InRange(Index)) {
                throw std::runtime_error("ERROR: Pointer array index is out of range!");
            }
            return *Array[Index];
        }

        /** Returns the data at the given index. */
        [[nodiscard]] Type &operator[](const int &Index) const {
            return Get(Index);
        }
    };
}
