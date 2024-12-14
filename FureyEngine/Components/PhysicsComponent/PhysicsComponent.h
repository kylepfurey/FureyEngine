// .h
// Physics Component Class Script
// by Kyle Furey

#pragma once
#include "../BrushComponent/BrushComponent.h"

#define DEFAULT_MASS 100

namespace FureyEngine {
    /** A component that provides its actor with physics movement that interacts with collision. */
    class PhysicsComponent : public Component {
    public:
        // PHYSICS

        /** A reference to a brush component used for this actor's collision detection. */
        Reference<BrushComponent> BrushComponent;

        /** The current speed to move the actor next tick. */
        glm::vec2 Velocity = {0, 0};

        /** The current speed to rotate the actor next tick. */
        float AngularVelocity = 0;

        /** The acceleration to constantly apply to the actor. */
        glm::vec2 Gravity = {0, -100};

        /** Whether gravity is enabled on this actor. */
        bool UseGravity = true;

        /** The speed to decrease velocity during collision. */
        float Friction = 50;

        /** A multiplier applied to velocity on collision. */
        float Bounciness = 0.1;

        /** The mass of this actor used for collision calculations. */
        float Mass = DEFAULT_MASS;

        /** A modifier to apply to movement updates. */
        float SpeedModifier = 1;

        /**
         * A delegate that is called when this actor collides with another.<br/>
         * The first parameter is a pointer to this physics component.<br/>
         * The second parameter is a pointer to the other brush component.
         */
        Delegate<PhysicsComponent *, FureyEngine::BrushComponent *> OnCollision;

        /** Whether to use LateTick() instead of Tick(). */
        bool TickLate = true;

        // CONSTRUCTORS

        /** Constructs a new physics component. */
        explicit PhysicsComponent(const Reference<FureyEngine::BrushComponent> &BrushComponent = nullref,
                                  const glm::vec2 &Gravity = {0, -100}, const bool &UseGravity = true,
                                  const float &Friction = 50, const float &Bounciness = 0.1, const float &Mass = DEFAULT_MASS,
                                  const float &SpeedModifier = 1, const bool &TickLate = true);

        // EVENTS

        /** Automatically called before each component has already called Start(). */
        void Attach() override;

        /** Automatically called each world tick. */
        void Tick(const double &DeltaTime) override;

        /** Automatically called after each component has already called Tick(). */
        void LateTick(const double &DeltaTime) override;

        /** Automatically called after this component is removed. */
        void Remove() override;

        // PHYSICS

        /** Returns this actor's current velocity. */
        [[nodiscard]] virtual glm::vec2 GetVelocity() const;

        /** Returns this actor's current angular velocity. */
        [[nodiscard]] virtual float GetAngularVelocity() const;

        /** Sets this actor's current velocity. */
        virtual void SetVelocity(const glm::vec2 &Velocity);

        /** Sets this actor's current angular velocity. */
        virtual void SetAngularVelocity(const float &AngularVelocity);

        /** Adds the given force to the actor's velocity and returns it. */
        virtual glm::vec2 AddForce(const glm::vec2 &Force);

        /** Adds the given force to the actor's angular velocity and returns it. */
        virtual float AddAngularForce(const float &Force);

        /** Zeroes out this actor's velocity and angular velocity. */
        virtual void Freeze();

        /**
         * Sweeps this actor to the given position and returns where it stopped.<br/>
         * This overload uses the given number of steps to check for collision.
         */
        virtual glm::vec2 SweepPositionTo(const glm::vec2 &Position, const int &Steps);

        /**
         * Sweeps this actor to the given position and returns where it stopped.<br/>
         * This overload uses a specific distance to calculate the number of steps used to check for collision.
         */
        virtual glm::vec2 SweepPositionTo(const glm::vec2 &Position, const float &StepDistance = 10);

        /**
         * Sweeps this actor to the given rotation and returns where it stopped.<br/>
         * This overload uses the given number of steps to check for collision.
         */
        virtual float SweepRotationTo(const float &Rotation, const int &Steps);

        /**
         * Sweeps this actor to the given rotation and returns where it stopped.<br/>
         * This overload uses a specific distance to calculate the number of steps used to check for collision.
         */
        virtual float SweepRotationTo(const float &Rotation, const float &StepDistance = 10);

        /**
         * Sweeps this actor to the given scale and returns where it stopped.<br/>
         * This overload uses the given number of steps to check for collision.
         */
        virtual glm::vec2 SweepScaleTo(const glm::vec2 &Scale, const int &Steps);

        /**
         * Sweeps this actor to the given scale and returns where it stopped.<br/>
         * This overload uses a specific distance to calculate the number of steps used to check for collision.
         */
        virtual glm::vec2 SweepScaleTo(const glm::vec2 &Scale, const float &StepDistance = 10);
    };
}
