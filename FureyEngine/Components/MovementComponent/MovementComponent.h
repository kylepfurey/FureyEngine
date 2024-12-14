// .h
// Movement Component Class Script
// by Kyle Furey

#pragma once
#include "../BrushComponent/BrushComponent.h"

#define MOVE_WITH_WASD 1
#define MOVE_WITH_ARROW_KEYS 1
#define MOVE_WITH_LEFT_STICK 1
#define MOVE_WITH_D_PAD 1

namespace FureyEngine {
    /** A component that provides simple customizable movement for the component's actor. */
    class MovementComponent : public Component {
    public:
        // MOVEMENT

        /** The maximum speed to move the actor on the X and Y axis. */
        glm::vec2 Speed = {75, 75};

        /** The speed to accelerate the actor to its max speed. */
        glm::vec2 Acceleration = {3, 3};

        /** The speed to reduce the actor's velocity to zero. */
        glm::vec2 Deceleration = {3, 3};

        /** The current speed to move this actor this tick. */
        glm::vec2 Velocity = {0, 0};

        /** Whether to use the keyboard to move the actor. */
        bool UseKeyboard = true;

        /** The index of the controller that drives the movement of this actor. */
        int ControllerIndex = -1;

        /** A reference to a brush component used for this actor's collision detection. */
        Reference<BrushComponent> BrushComponent;

        /** Whether the movement direction is relative to the actor's rotation. */
        bool RelativeToRotation = false;

        /** A modifier to apply to movement updates. */
        float SpeedModifier = 1;

        /** Whether to use LateTick() instead of Tick(). */
        bool TickLate = false;

        // CONSTRUCTORS

        /** Constructs a new movement component. */
        explicit MovementComponent(const glm::vec2 &Speed = {75, 75},
                                   const glm::vec2 &Acceleration = {3, 3},
                                   const int &ControllerIndex = -1,
                                   const Reference<FureyEngine::BrushComponent> &BrushComponent = nullref,
                                   const bool &RelativeToRotation = false, const float &SpeedModifier = 1,
                                   const bool &TickLate = false);

        // EVENTS

        /** Automatically called before each component has already called Start(). */
        void Attach() override;

        /** Automatically called each world tick. */
        void Tick(const double &DeltaTime) override;

        /** Automatically called after each component has already called Tick(). */
        void LateTick(const double &DeltaTime) override;

        /** Automatically called after this component is removed. */
        void Remove() override;
    };
}
