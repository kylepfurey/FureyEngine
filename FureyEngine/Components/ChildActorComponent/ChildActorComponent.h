// .h
// Child Actor Component Class Script
// by Kyle Furey

#pragma once
#include "../../Engine/Engine.h"

namespace FureyEngine {
    /** A component that forces an actor to follow another actor's transform relatively. */
    class ChildActorComponent : public Component {
    protected:
        // PARENTING

        /** The actor to parent this actor to. */
        Reference<Actor> MyParent = nullref;

        /** The world transform of this component's actor the previous tick. */
        Transform LastWorldTransform = Transform();

    public:
        // PARENTING

        /** Transform values to use when applying this actor's transform to its parent's. */
        Transform RelativeTransform = Transform();

        /** Whether to use LateTick() instead of Tick(). */
        bool TickLate = true;


        // CONSTRUCTORS

        /** Constructs a new child actor component. */
        explicit ChildActorComponent(const Reference<Actor> &ParentActor = nullref,
                                     const bool &TickLate = true);

        /** Constructs a new child actor component with the given transform. */
        explicit ChildActorComponent(const Reference<Actor> &ParentActor,
                                     const Transform &RelativeTransform,
                                     const bool &TickLate = true);

        // EVENTS

        /** Automatically called before each component has already called Start(). */
        void Attach() override;

        /** Automatically called each world tick. */
        void Tick(const double &DeltaTime) override;

        /** Automatically called after each component has already called Tick(). */
        void LateTick(const double &DeltaTime) override;

        /** Automatically called after this component is removed. */
        void Remove() override;

        // GETTERS

        /** Returns this component's actor's parent actor. */
        [[nodiscard]] virtual Reference<Actor> GetParent() const;

        /** Returns this component's actor's transform relative to its parent actor. */
        [[nodiscard]] virtual Transform GetRelativeTransform() const;

        /** Returns this component's actor's position relative to its parent actor. */
        [[nodiscard]] virtual glm::vec2 GetRelativePosition() const;

        /** Returns this component's actor's rotation relative to its parent actor. */
        [[nodiscard]] virtual float GetRelativeRotation() const;

        /** Returns this component's actor's scale relative to its parent actor. */
        [[nodiscard]] virtual glm::vec2 GetRelativeScale() const;

        /**
         * Returns a world transform calculated from this actor's component's transform<br/>
         * relative to its parent actor, accounting for its possible parent actors as well.
         */
        [[nodiscard]] virtual Transform GetWorldTransform() const;

        /**
         * Returns a world position calculated from this actor's component's transform<br/>
         * relative to its parent actor, accounting for its possible parent actors as well.
         */
        [[nodiscard]] virtual glm::vec2 GetWorldPosition() const;

        /**
         * Returns a world rotation calculated from this actor's component's transform<br/>
         * relative to its parent actor, accounting for its possible parent actors as well.
         */
        [[nodiscard]] virtual float GetWorldRotation() const;

        /**
         * Returns a world scale calculated from this actor's component's transform<br/>
         * relative to its parent actor, accounting for its possible parent actors as well.
         */
        [[nodiscard]] virtual glm::vec2 GetWorldScale() const;

        /**
        * Converts this component's actor's world transform into a transform relative to the given transform.<br/>
        * This new transform is as if this component's actor's origin was that transform.
        */
        [[nodiscard]] virtual Transform GetTransformRelativeTo(const Transform &ParentTransform) const;

        // SETTERS

        /**
         * Reparents this component's actor to the given actor.<br/>
         * This component's actor's relative transform will change to match its current world transform.<br/>
         * This returns true unless the parent actor is invalid.
         */
        virtual bool SetParent(const Reference<Actor> &ParentActor);

        /**
         * Reparents this component's actor to the given actor with the given relative transform.<br/>
         * This returns true unless the parent actor is invalid.
         */
        virtual bool SetParent(const Reference<Actor> &ParentActor, const Transform &RelativeTransform);

        /** Sets the relative transform of this component's actor. */
        virtual void SetRelativeTransform(const Transform &RelativeTransform);

        /** Sets the relative position of this component's actor. */
        virtual void SetRelativePosition(const glm::vec2 &RelativePosition);

        /**  Sets the relative rotation of this component's actor. */
        virtual void SetRelativeRotation(const float &RelativeRotation);

        /**  Sets the relative scale of this component's actor. */
        virtual void SetRelativeScale(const glm::vec2 &RelativeScale);

        /** Sets the transform of this component's actor to the given world transform, and updates its relative transform. */
        virtual void SetWorldTransform(const Transform &WorldTransform);

        /** Sets the position of this component's actor to the given world position, and updates its relative transform. */
        virtual void SetWorldPosition(const glm::vec2 &WorldPosition);

        /** Sets the rotation of this component's actor to the given world rotation, and updates its relative transform. */
        virtual void SetWorldRotation(const float &WorldRotation);

        /** Sets the scale of this component's actor to the given world scale, and updates its relative transform. */
        virtual void SetWorldScale(const glm::vec2 &WorldScale);
    };
}
