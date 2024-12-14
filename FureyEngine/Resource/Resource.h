// .h
// Engine Resource Class Script
// by Kyle Furey

#pragma once
#include "../Standard/Standard.h"

namespace FureyEngine {
    /** Stores newly allocated instances of a type by name for easy public access. */
    template<typename ValueType>
    class Resource final {
        // RESOURCE MAP

        /** The underlying map for this resource. */
        std::map<std::string, ValueType> MyResource;

    public:
        // NEW RESOURCE

        /** Constructs and stores a new resource. */
        template<typename... ArgumentTypes>
        ValueType &New(const std::string &Key, ArgumentTypes... Arguments) {
            if (MyResource.count(Key)) {
                throw std::runtime_error(std::string("ERROR: Engine resource ") + Key + " already exists!");
            }
            MyResource.try_emplace(Key, Arguments...);
            return MyResource.at(Key);
        }

        /** Constructs and stores a new resource. */
        template<typename... ArgumentTypes>
        ValueType &operator()(const std::string &Key, ArgumentTypes... Arguments) {
            return New(Key, Arguments...);
        }

        // LOAD RESOURCE

        /** Loads the given resource by name. */
        [[nodiscard]] ValueType &Load(const std::string &Key) {
            if (!MyResource.count(Key)) {
                throw std::runtime_error(std::string("ERROR: Engine resource ") + Key + " does not exist!");
            }
            return MyResource.at(Key);
        }

        /** Loads the given resource by name. */
        [[nodiscard]] ValueType &operator[](const std::string &Key) {
            return Load(Key);
        }

        // UNLOAD RESOURCE

        /** Removes the given resource by name. */
        void Unload(const std::string &Key) {
            if (!MyResource.count(Key)) {
                throw std::runtime_error(std::string("ERROR: Engine resource ") + Key + " does not exist!");
            }
            MyResource.erase(Key);
        }

        /** Clears all of this resource. */
        void Clear() {
            MyResource.clear();
        }

        // TRACKING RESOURCE

        /** Returns the total number of instances of this resource. */
        [[nodiscard]] int Count() {
            return static_cast<int>(MyResource.size());
        }

        /** Returns whether the given resource is present. */
        [[nodiscard]] bool Contains(const std::string &Key) {
            return MyResource.count(Key);
        }
    };
}
