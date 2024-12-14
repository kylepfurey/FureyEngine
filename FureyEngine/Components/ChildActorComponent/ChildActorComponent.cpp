// .cpp
// Child Actor Component Class Script
// by Kyle Furey

#include "ChildActorComponent.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Constructs a new child actor component.
    ChildActorComponent::ChildActorComponent(const Reference<Actor> &ParentActor,
                                             const bool &TickLate)
        : Component(std::string("Child of ") + (ParentActor ? ParentActor->Name : "NULL"), true),
          MyParent(ParentActor), RelativeTransform(Transform({FLT_MAX, FLT_MAX})), TickLate(TickLate) {
    }

    // Constructs a new child actor component with the given transform.
    ChildActorComponent::ChildActorComponent(const Reference<Actor> &ParentActor,
                                             const Transform &RelativeTransform,
                                             const bool &TickLate)
        : Component(std::string("Child of ") + (ParentActor ? ParentActor->Name : "NULL"), true),
          MyParent(ParentActor), RelativeTransform(RelativeTransform), TickLate(TickLate) {
    }

    // EVENTS

    // Automatically called before each component has already called Start().
    void ChildActorComponent::Attach() {
        // Calls the base class's function
        Component::Attach();

        // Bind set active to the actor's set active function
        GetActor()->OnSetActive.Bind(std::bind(ChildActorComponent::SetActive, this, std::placeholders::_1));

        // Parent the component's actor to its parent actor
        if (RelativeTransform.Position.x == FLT_MAX && RelativeTransform.Position.y == FLT_MAX) {
            SetParent(MyParent);
        } else {
            SetParent(MyParent, RelativeTransform);
        }
    }

    // Automatically called each world tick.
    void ChildActorComponent::Tick(const double &DeltaTime) {
        // Calls the base class's function
        Component::Tick(DeltaTime);

        // Update this actor's transform based on its relative transform
        if (!TickLate && MyParent) {
            if (GetActor()->Transform != LastWorldTransform) {
                RelativeTransform = GetTransformRelativeTo(MyParent->Transform);
            } else {
                GetActor()->Transform = GetWorldTransform();
            }
            LastWorldTransform = GetActor()->Transform;
        }
    }

    // Automatically called after each component has already called Tick().
    void ChildActorComponent::LateTick(const double &DeltaTime) {
        // Calls the base class's function
        Component::LateTick(DeltaTime);

        // Update this actor's transform based on its relative transform
        if (TickLate && MyParent) {
            if (GetActor()->Transform != LastWorldTransform) {
                RelativeTransform = GetTransformRelativeTo(MyParent->Transform);
            } else {
                GetActor()->Transform = GetWorldTransform();
            }
            LastWorldTransform = GetActor()->Transform;
        }
    }

    // Automatically called after this component is removed.
    void ChildActorComponent::Remove() {
        // Calls the base class's function
        Component::Remove();

        // Unbind set active from the actor's set active function
        if (BeginTick) {
            GetActor()->OnSetActive.Unbind(std::bind(ChildActorComponent::SetActive, this, std::placeholders::_1));
        }
    }

    // GETTERS

    // Returns this component's actor's parent actor.
    Reference<Actor> ChildActorComponent::GetParent() const {
        return MyParent;
    }

    // Returns this component's actor's transform relative to its parent actor.
    Transform ChildActorComponent::GetRelativeTransform() const {
        return RelativeTransform;
    }

    // Returns this component's actor's position relative to its parent actor.
    glm::vec2 ChildActorComponent::GetRelativePosition() const {
        return RelativeTransform.Position;
    }

    // Returns this component's actor's rotation relative to its parent actor.
    float ChildActorComponent::GetRelativeRotation() const {
        return RelativeTransform.Rotation;
    }

    // Returns this component's actor's scale relative to its parent actor.
    glm::vec2 ChildActorComponent::GetRelativeScale() const {
        return RelativeTransform.Scale;
    }

    // Returns a world transform calculated from this component's actor's transform
    // relative to its parent actor, accounting for its possible parent actors as well.
    Transform ChildActorComponent::GetWorldTransform() const {
        if (!MyParent) {
            return RelativeTransform;
        }
        auto ParentTransform = MyParent->Transform;
        if (const auto ParentComponent = MyParent->GetComponent<ChildActorComponent>()) {
            ParentTransform = ParentComponent->GetWorldTransform();
        }
        return Transform::RelativeToWorld(RelativeTransform, ParentTransform);
    }

    // Returns a world position calculated from this actor's component's transform
    // relative to its parent actor, accounting for its possible parent actors as well.
    glm::vec2 ChildActorComponent::GetWorldPosition() const {
        return GetWorldTransform().Position;
    }

    // Returns a world rotation calculated from this actor's component's transform
    // relative to its parent actor, accounting for its possible parent actors as well.
    float ChildActorComponent::GetWorldRotation() const {
        return GetWorldTransform().Rotation;
    }

    // Returns a world scale calculated from this actor's component's transform
    // relative to its parent actor, accounting for its possible parent actors as well.
    glm::vec2 ChildActorComponent::GetWorldScale() const {
        return GetWorldTransform().Scale;
    }

    // Converts this component's actor's world transform into a transform relative to the given transform.
    // This new transform is as if this component's actor's origin was that transform.
    Transform ChildActorComponent::GetTransformRelativeTo(const Transform &ParentTransform) const {
        return Transform::WorldToRelative(GetActor()->Transform, ParentTransform);
    }

    // SETTERS

    // Reparents this component's actor to the given actor.
    // This component's actor's relative transform will change to match its current world transform.
    // This returns true unless the parent actor is invalid.
    bool ChildActorComponent::SetParent(const Reference<Actor> &ParentActor) {
        if (ParentActor && ParentActor != GetActor()) {
            MyParent = ParentActor;
            RelativeTransform = GetTransformRelativeTo(ParentActor->Transform);
            GetActor()->Transform = GetWorldTransform();
            LastWorldTransform = GetActor()->Transform;
            Name = std::string("Child of ") + ParentActor->Name;
            return true;
        }
        MyParent = nullref;
        Name = "Child of NULL";
        LastWorldTransform = GetActor()->Transform;
        return false;
    }

    // Reparents this component's actor to the given actor with the given relative transform.
    // This returns true unless the parent actor is invalid.
    bool ChildActorComponent::SetParent(const Reference<Actor> &ParentActor, const Transform &RelativeTransform) {
        if (ParentActor && ParentActor != GetActor()) {
            MyParent = ParentActor;
            this->RelativeTransform = RelativeTransform;
            GetActor()->Transform = GetWorldTransform();
            LastWorldTransform = GetActor()->Transform;
            Name = std::string("Child of ") + ParentActor->Name;
            return true;
        }
        MyParent = nullref;
        Name = "Child of NULL";
        LastWorldTransform = GetActor()->Transform;
        return false;
    }

    // Sets the relative transform of this component's actor.
    void ChildActorComponent::SetRelativeTransform(const Transform &RelativeTransform) {
        this->RelativeTransform = RelativeTransform;
        GetActor()->Transform = GetWorldTransform();
        LastWorldTransform = GetActor()->Transform;
    }

    // Sets the relative position of this component's actor.
    void ChildActorComponent::SetRelativePosition(const glm::vec2 &RelativePosition) {
        RelativeTransform.Position = RelativePosition;
        GetActor()->Transform = GetWorldTransform();
        LastWorldTransform = GetActor()->Transform;
    }

    // Sets the relative rotation of this component's actor.
    void ChildActorComponent::SetRelativeRotation(const float &RelativeRotation) {
        RelativeTransform.Rotation = RelativeRotation;
        GetActor()->Transform = GetWorldTransform();
        LastWorldTransform = GetActor()->Transform;
    }

    // Sets the relative scale of this component's actor.
    void ChildActorComponent::SetRelativeScale(const glm::vec2 &RelativeScale) {
        RelativeTransform.Scale = RelativeScale;
        GetActor()->Transform = GetWorldTransform();
        LastWorldTransform = GetActor()->Transform;
    }

    // Sets the transform of this component's actor to the given world transform, and updates its relative transform.
    void ChildActorComponent::SetWorldTransform(const Transform &WorldTransform) {
        GetActor()->Transform = WorldTransform;
        if (MyParent) {
            RelativeTransform = GetTransformRelativeTo(MyParent->Transform);
        }
        LastWorldTransform = GetActor()->Transform;
    }

    // Sets the position of this component's actor to the given world position, and updates its relative transform.
    void ChildActorComponent::SetWorldPosition(const glm::vec2 &WorldPosition) {
        GetActor()->Transform.Position = WorldPosition;
        if (MyParent) {
            RelativeTransform = GetTransformRelativeTo(MyParent->Transform);
        }
        LastWorldTransform = GetActor()->Transform;
    }

    // Sets the rotation of this component's actor to the given world rotation, and updates its relative transform.
    void ChildActorComponent::SetWorldRotation(const float &WorldRotation) {
        GetActor()->Transform.Rotation = WorldRotation;
        if (MyParent) {
            RelativeTransform = GetTransformRelativeTo(MyParent->Transform);
        }
        LastWorldTransform = GetActor()->Transform;
    }

    // Sets the scale of this component's actor to the given world scale, and updates its relative transform.
    void ChildActorComponent::SetWorldScale(const glm::vec2 &WorldScale) {
        GetActor()->Transform.Scale = WorldScale;
        if (MyParent) {
            RelativeTransform = GetTransformRelativeTo(MyParent->Transform);
        }
        LastWorldTransform = GetActor()->Transform;
    }
}
