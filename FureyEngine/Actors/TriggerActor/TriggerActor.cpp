// .cpp
// Trigger Box Actor Class Script
// by Kyle Furey

#include "TriggerActor.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Constructs a new trigger box with a name.
    TriggerActor::TriggerActor(const std::string &Name, const bool &Active)
        : ColliderActor(Name, Active) {
    }

    // TRIGGER

    // A function used to dispatch trigger collision events.
    void TriggerActor::OnCollision(const FureyEngine::BrushComponent *MyBrushComponent,
                                   FureyEngine::BrushComponent *OtherBrushComponent) {
        if (MyBrushComponent->CollisionStateWith(OtherBrushComponent) ==
            BrushComponent::CollisionState::COLLISION_ENTER) {
            OnTriggerEnter(this, OtherBrushComponent);
        } else if (MyBrushComponent->CollisionStateWith(OtherBrushComponent) ==
                   BrushComponent::CollisionState::COLLISION_EXIT) {
            OnTriggerExit(this, OtherBrushComponent);
        }
    }

    // Initializes this trigger box with the given transform.
    Reference<ColliderActor> TriggerActor::Initialize(const FureyEngine::Transform &Bounds,
                                                      const std::string &Layer) {
        if (!Initialized) {
            // Calls the base class's function
            const auto Me = ColliderActor::Initialize(Bounds, Layer);

            BrushComponent->Trigger = true;
            BrushComponent->OnCollision.Bind(
                [this](const FureyEngine::BrushComponent *MyBrushComponent,
                       FureyEngine::BrushComponent *OtherBrushComponent) {
                    OnCollision(MyBrushComponent, OtherBrushComponent);
                });
            return Me;
        }

        // Calls the base class's function
        return ColliderActor::Initialize(Bounds, Layer);
    }

    // Initializes this trigger box with the given size.
    Reference<ColliderActor> TriggerActor::Initialize(const glm::vec2 &Size,
                                                      const std::string &Layer) {
        if (!Initialized) {
            // Calls the base class's function
            const auto Me = ColliderActor::Initialize(Size, Layer);

            BrushComponent->Trigger = true;
            BrushComponent->OnCollision.Bind(
                [this](const FureyEngine::BrushComponent *MyBrushComponent,
                       FureyEngine::BrushComponent *OtherBrushComponent) {
                    OnCollision(MyBrushComponent, OtherBrushComponent);
                });
            return Me;
        }

        // Calls the base class's function
        return ColliderActor::Initialize(Size, Layer);
    }

    // Initializes this trigger box with the given render component.
    Reference<ColliderActor> TriggerActor::Initialize(const Reference<RenderComponent> &RenderComponent,
                                                      const std::string &Layer) {
        if (!Initialized) {
            // Calls the base class's function
            const auto Me = ColliderActor::Initialize(RenderComponent, Layer);

            BrushComponent->Trigger = true;
            BrushComponent->OnCollision.Bind(
                [this](const FureyEngine::BrushComponent *MyBrushComponent,
                       FureyEngine::BrushComponent *OtherBrushComponent) {
                    OnCollision(MyBrushComponent, OtherBrushComponent);
                });
            return Me;
        }

        // Calls the base class's function
        return ColliderActor::Initialize(RenderComponent, Layer);
    }

    // Initializes this trigger box with the given animation component.
    Reference<ColliderActor> TriggerActor::Initialize(const Reference<AnimationComponent> &AnimationComponent,
                                                      const std::string &Layer) {
        if (!Initialized) {
            // Calls the base class's function
            const auto Me = ColliderActor::Initialize(AnimationComponent, Layer);

            BrushComponent->Trigger = true;
            BrushComponent->OnCollision.Bind(
                [this](const FureyEngine::BrushComponent *MyBrushComponent,
                       FureyEngine::BrushComponent *OtherBrushComponent) {
                    OnCollision(MyBrushComponent, OtherBrushComponent);
                });
            return Me;
        }

        // Calls the base class's function
        return ColliderActor::Initialize(AnimationComponent, Layer);
    }
}
