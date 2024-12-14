// .cpp
// OpenGL Transform Types Script
// by Kyle Furey

#include "Transform.h"
#include "../Math/Math.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Constructs a new transform.
    Transform::Transform(const glm::vec2 &Position, const float &Rotation, const glm::vec2 &Scale)
        : Position(Position), Rotation(Rotation), Scale(Scale) {
    }

    // MATRIX

    // Converts this transform into a 4 x 4 matrix.
    glm::mat4 Transform::GetMatrix() const {
        auto Matrix = glm::mat4(1);
        Matrix = glm::translate(Matrix, glm::vec3(Position.x, Position.y, 0));
        Matrix = glm::rotate(Matrix, Rotation, glm::vec3(0, 0, 1));
        Matrix = glm::scale(Matrix, glm::vec3(Scale.x, Scale.y, 1));
        return Matrix;
    }

    // EQUALITY

    // Returns whether the given transforms are equal.
    bool Transform::operator==(const Transform &Other) const {
        return Position.x == Other.Position.x &&
               Position.y == Other.Position.y &&
               Rotation == Other.Rotation &&
               Scale.x == Other.Scale.x &&
               Scale.y == Other.Scale.y;
    }

    // Returns whether the given transforms are not equal.
    bool Transform::operator!=(const Transform &Other) const {
        return Position.x != Other.Position.x ||
               Position.y != Other.Position.y ||
               Rotation != Other.Rotation ||
               Scale.x != Other.Scale.x ||
               Scale.y != Other.Scale.y;
    }

    // DIRECTION

    // Returns this transform's normalized up direction based on its rotation.
    glm::vec2 Transform::Up() const {
        const double Radians = glm::radians(Rotation);
        const auto X = static_cast<float>(glm::sin(Radians));
        const auto Y = static_cast<float>(glm::cos(Radians));
        return {X, Y};
    }

    // Returns this transform's normalized down direction based on its rotation.
    glm::vec2 Transform::Down() const {
        const double Radians = glm::radians(Rotation);
        const auto X = static_cast<float>(-glm::sin(Radians));
        const auto Y = static_cast<float>(-glm::cos(Radians));
        return {X, Y};
    }

    // Returns this transform's normalized left direction based on its rotation.
    glm::vec2 Transform::Left() const {
        const double Radians = glm::radians(Rotation);
        const auto X = static_cast<float>(-glm::cos(Radians));
        const auto Y = static_cast<float>(glm::sin(Radians));
        return {X, Y};
    }

    // Returns this transform's normalized right direction based on its rotation.
    glm::vec2 Transform::Right() const {
        const double Radians = glm::radians(Rotation);
        const auto X = static_cast<float>(glm::cos(Radians));
        const auto Y = static_cast<float>(-glm::sin(Radians));
        return {X, Y};
    }

    // RELATIVITY

    // Returns a relative transform of the given world transform relative to the given parent transform.
    Transform Transform::WorldToRelative(Transform WorldTransform, const Transform &ParentTransform) {
        // Adjust position
        WorldTransform.Position.x -= ParentTransform.Position.x;
        WorldTransform.Position.y -= ParentTransform.Position.y;

        // Adjust rotation
        WorldTransform.Rotation -= ParentTransform.Rotation;

        // Inverse rotate the position
        WorldTransform.Position = Math::Rotate(WorldTransform.Position, -ParentTransform.Rotation);

        // Adjust scale
        if (ParentTransform.Scale.x != 0) {
            WorldTransform.Scale.x /= ParentTransform.Scale.x;
            WorldTransform.Position.x /= ParentTransform.Scale.x;
        } else {
            WorldTransform.Scale.x = 0;
            WorldTransform.Position.x = 0;
        }
        if (ParentTransform.Scale.y != 0) {
            WorldTransform.Scale.y /= ParentTransform.Scale.y;
            WorldTransform.Position.y /= ParentTransform.Scale.y;
        } else {
            WorldTransform.Scale.y = 0;
            WorldTransform.Position.y = 0;
        }

        return WorldTransform;
    }

    // Returns a world transform of the given relative transform relative to the given parent transform.
    Transform Transform::RelativeToWorld(Transform RelativeTransform, const Transform &ParentTransform) {
        // Adjust scale
        RelativeTransform.Scale.x *= ParentTransform.Scale.x;
        RelativeTransform.Scale.y *= ParentTransform.Scale.y;
        RelativeTransform.Position.x *= ParentTransform.Scale.x;
        RelativeTransform.Position.y *= ParentTransform.Scale.y;

        // Rotate the position
        RelativeTransform.Position = Math::Rotate(RelativeTransform.Position, ParentTransform.Rotation);

        // Adjust rotation
        RelativeTransform.Rotation += ParentTransform.Rotation;

        // Adjust position
        RelativeTransform.Position.x += ParentTransform.Position.x;
        RelativeTransform.Position.y += ParentTransform.Position.y;

        return RelativeTransform;
    }
}
