// .h
// Delegate Type Script
// by Kyle Furey

#pragma once
#include "../Standard/Standard.h"

namespace FureyEngine {
    /**
     * Represents a collection of void returning functions that can be bound<br/>
     * and called simultaneously with parameters.
     */
    template<typename... ArgumentTypes>
    class Delegate {
        // FUNCTIONS

        /** Each of the functions bound to this delegate. */
        std::vector<std::function<void(ArgumentTypes...)> > Functions;

    public:
        // BIND

        /** Binds the given function to this delegate. */
        void Bind(const std::function<void(ArgumentTypes...)> &Function) {
            if (Function != nullptr) {
                Functions.push_back(Function);
            } else {
                throw std::runtime_error("ERROR: Attempted to bind an invalid function to the delegate!");
            }
        }

        /** Binds the given function to this delegate. */
        Delegate &operator+=(const std::function<void(ArgumentTypes...)> &Function) {
            Bind(Function);
            return *this;
        }

        // UNBIND

        /** Unbinds the given function from this delegate. */
        void Unbind(const std::function<void(ArgumentTypes...)> &Function) {
            for (int i = 0; i < Functions.size(); i++) {
                if (Functions[i].template target<void(ArgumentTypes...)>() ==
                    Function.template target<void(ArgumentTypes...)>()) {
                    Functions.erase(Functions.begin() + i);
                    return;
                }
            }
            throw std::runtime_error("ERROR: Attempted to unbind a non-existent function from the delegate!");
        }

        /** Unbinds the given function from this delegate. */
        Delegate &operator-=(const std::function<void(ArgumentTypes...)> &Function) {
            Unbind(Function);
            return *this;
        }

        // INVOKE

        /** Invokes all of the functions for this delegate. */
        void Invoke(ArgumentTypes... Arguments) const {
            for (const auto &Function: Functions) {
                if (Function != nullptr) {
                    Function(Arguments...);
                } else {
                    throw std::runtime_error("ERROR: Attempted to invoke an invalid function in the delegate!");
                }
            }
        }

        /** Invokes all of the functions for this delegate. */
        Delegate &operator()(ArgumentTypes... Arguments) {
            Invoke(Arguments...);
            return *this;
        }

        // COUNT

        /** Returns the number of bound functions to this delegate. */
        [[nodiscard]] int Count() const {
            return static_cast<int>(Functions.size());
        }

        // CLEAR

        /** Clears all functions bound to this delegate. */
        void Clear() {
            if (!Functions.empty()) {
                Functions.clear();
            }
        }
    };
}
