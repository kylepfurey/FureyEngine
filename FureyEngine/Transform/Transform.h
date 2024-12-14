// .h
// OpenGL Transform Types Script
// by Kyle Furey

#pragma once
#include "../Standard/Standard.h"

namespace FureyEngine {
    /** Represents an object's transform in space. */
    struct Transform {
        // TRANSFORM

        /** This object's position (relative to the world). */
        glm::vec2 Position;

        /** This object's rotation (in degrees). */
        float Rotation;

        /** This object's scale (relative to its pixel size). */
        glm::vec2 Scale;

        // CONSTRUCTORS

        /** Constructs a new transform. */
        explicit Transform(const glm::vec2 &Position = {0, 0},
                           const float &Rotation = {0},
                           const glm::vec2 &Scale = {1, 1});

        // MATRIX

        /** Converts this transform into a 4 x 4 matrix. */
        [[nodiscard]] glm::mat4 GetMatrix() const;

        // EQUALITY

        /** Returns whether the given transforms are equal. */
        [[nodiscard]] bool operator ==(const Transform &Other) const;

        /** Returns whether the given transforms are not equal. */
        [[nodiscard]] bool operator !=(const Transform &Other) const;

        // DIRECTION

        /** Returns this transform's normalized up direction based on its rotation. */
        [[nodiscard]] glm::vec2 Up() const;

        /** Returns this transform's normalized up direction based on its rotation. */
        [[nodiscard]] glm::vec2 Right() const;

        /** Returns this transform's normalized up direction based on its rotation. */
        [[nodiscard]] glm::vec2 Left() const;

        /** Returns this transform's normalized up direction based on its rotation. */
        [[nodiscard]] glm::vec2 Down() const;

        // RELATIVITY

        /** Returns a relative transform of the given world transform relative to the given parent transform. */
        [[nodiscard]] static Transform WorldToRelative(Transform WorldTransform, const Transform &ParentTransform);

        /** Returns a world transform of the given relative transform relative to the given parent transform. */
        [[nodiscard]] static Transform RelativeToWorld(Transform RelativeTransform, const Transform &ParentTransform);
    };
}
