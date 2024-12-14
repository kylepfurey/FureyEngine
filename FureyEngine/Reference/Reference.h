// .h
// Weak Reference Class Script
// by Kyle Furey

#pragma once
#include "../Standard/Standard.h"

// Represents a reference with no associated value.
#define nullref {}

namespace FureyEngine {
    /**
     * Represents a weak pointer to an object.<br/>
     * You can use these to store references to a component, an actor, or a world without extending its lifetime.
     */
    template<typename Type>
    class Reference {
        // DATA

        /** The underlying weak pointer of this class. */
        std::weak_ptr<Type> Pointer;

    public:
        // CONSTRUCTORS

        /** Constructs a new reference from a weak pointer. */
        Reference(const std::weak_ptr<Type> &Pointer = nullref) : Pointer(Pointer) {
        }

        /** Constructs a new reference from a shared pointer. */
        Reference(const std::shared_ptr<Type> &Pointer) : Pointer(Pointer) {
        }

        /** Constructs a new reference from a raw pointer. */
        Reference(Type *Pointer) : Pointer(std::dynamic_pointer_cast<Type>(Pointer->shared_from_this())) {
        }

        // CONVERSIONS

        /** Converts this reference into a weak pointer. */
        explicit operator std::weak_ptr<Type> *&() {
            return Pointer;
        }

        /** Converts this reference into a raw pointer. */
        explicit operator Type *() {
            if (Pointer.expired()) {
                throw std::runtime_error("ERROR: Attempting to dereference a null reference!\n"
                    "You need check if the reference is still valid before using it!");
            }
            return Pointer.lock().get();
        }

        // DEREFERENCE

        /** Converts this reference into a raw pointer. */
        [[nodiscard]] Type &operator*() const {
            if (Pointer.expired()) {
                throw std::runtime_error("ERROR: Attempting to dereference a null reference!\n"
                    "You need check if the reference is still valid before using it!");
            }
            return *Pointer.lock().get();
        }

        /** Converts this reference into a raw pointer. */
        [[nodiscard]] Type *operator ->() const {
            if (Pointer.expired()) {
                throw std::runtime_error("ERROR: Attempting to dereference a null reference!\n"
                    "You need check if the reference is still valid before using it!");
            }
            return Pointer.lock().get();
        }

        // COMPARISON

        /** Returns whether the given references are equal. */
        [[nodiscard]] bool operator==(const Reference &Other) const {
            if (Pointer.expired()) {
                return Other.Pointer.expired();
            }
            if (Other.Pointer.expired()) {
                return false;
            }
            return Pointer.lock() == Other.Pointer.lock();
        }

        /** Returns whether the given references are not equal. */
        [[nodiscard]] bool operator!=(const Reference &Other) const {
            if (Pointer.expired()) {
                return !Other.Pointer.expired();
            }
            if (Other.Pointer.expired()) {
                return true;
            }
            return Pointer.lock() != Other.Pointer.lock();
        }

        // SAFETY

        /** Returns whether this reference is still valid. */
        [[nodiscard]] explicit operator bool() const {
            return !Pointer.expired();
        }
    };
}
