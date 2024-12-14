// .h
// Event Type Script
// by Kyle Furey

#pragma once
#include "../Standard/Standard.h"

namespace FureyEngine {
    /**
     * A collection of void returning functions that can be bound<br/>
     * and called simultaneously.
     */
    class Event {
        // FUNCTIONS

        /** Each of the functions bound to this event. */
        std::vector<std::function<void()> > Functions;

    public:
        // BIND

        /** Binds the given function to this event. */
        void Bind(const std::function<void()> &Function);

        /** Binds the given function to this event. */
        Event &operator+=(const std::function<void()> &Function);

        // UNBIND

        /** Unbinds the given function from this event. */
        void Unbind(const std::function<void()> &Function);

        /** Unbinds the given function from this event. */
        Event &operator-=(const std::function<void()> &Function);

        // INVOKE

        /** Invokes all of the functions for this event. */
        void Invoke() const;

        /** Invokes all of the functions for this event. */
        Event &operator()();

        // COUNT

        /** Returns the number of bound functions to this event. */
        [[nodiscard]] int Count() const;

        // CLEAR

        /** Clears all functions bound to this event. */
        void Clear();
    };
}
