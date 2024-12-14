// .cpp
// Event Type Script
// by Kyle Furey

#include "Event.h"

namespace FureyEngine {
    // BIND

    // Binds the given function to this event.
    void Event::Bind(const std::function<void()> &Function) {
        if (Function != nullptr) {
            Functions.push_back(Function);
        } else {
            throw std::runtime_error("ERROR: Attempted to bind an invalid function to the event!");
        }
    }

    // Binds the given function to this event.
    Event &Event::operator+=(const std::function<void()> &Function) {
        Bind(Function);
        return *this;
    }

    // UNBIND

    // Unbinds the given function from this event.
    void Event::Unbind(const std::function<void()> &Function) {
        for (int i = 0; i < Functions.size(); i++) {
            if (Functions[i].target<void()>() == Function.target<void()>()) {
                Functions.erase(Functions.begin() + i);
                return;
            }
        }
        throw std::runtime_error("ERROR: Attempted to unbind a non-existent function from the event!");
    }

    // Unbinds the given function from this event.
    Event &Event::operator-=(const std::function<void()> &Function) {
        Unbind(Function);
        return *this;
    }

    // INVOKE

    // Invokes all of the functions for this event.
    void Event::Invoke() const {
        for (const auto &Function: Functions) {
            if (Function != nullptr) {
                Function();
            } else {
                throw std::runtime_error("ERROR: Attempted to invoke an invalid function in the event!");
            }
        }
    }

    // Invokes all of the functions for this event.
    Event &Event::operator()() {
        Invoke();
        return *this;
    }

    // COUNT

    // Returns the number of bound functions to this event.
    int Event::Count() const {
        return static_cast<int>(Functions.size());
    }

    // CLEAR

    // Clears all functions bound to this event.
    void Event::Clear() {
        if (!Functions.empty()) {
            Functions.clear();
        }
    }
}
