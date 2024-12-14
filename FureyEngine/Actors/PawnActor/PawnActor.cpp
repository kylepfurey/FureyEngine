// .cpp
// Player Pawn Actor Class Script
// by Kyle Furey

#include "PawnActor.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Constructs a new player pawn with a name.
    PawnActor::PawnActor(const std::string &Name, const bool &Active)
        : Actor(Name, Active) {
    }

    // PAWN

    // Returns whether this player pawn has been initialized yet.
    bool PawnActor::IsInitialized() const {
        return Initialized;
    }

    // Initializes this player pawn's components with an image.
    Reference<PawnActor> PawnActor::Initialize(Image *Image,
                                               const bool &Collides, const bool &Physics) {
        if (!Initialized) {
            Initialized = true;
            RenderComponent = AttachComponent<FureyEngine::RenderComponent>(Image);
            CameraComponent = AttachComponent<FureyEngine::CameraComponent>();
            if (Collides) {
                BrushComponent = AttachComponent<FureyEngine::BrushComponent>(RenderComponent);
            }
            MovementComponent = AttachComponent<FureyEngine::MovementComponent>();
            if (Physics) {
                PhysicsComponent = AttachComponent<FureyEngine::PhysicsComponent>(BrushComponent);
            }
        }
        return {this};
    }

    // Initializes this player pawn's components with an animation.
    Reference<PawnActor> PawnActor::Initialize(const std::string &CurrentAnimation, Animation *Animation,
                                               const bool &Collides, const bool &Physics) {
        if (!Initialized) {
            Initialized = true;
            RenderComponent = AttachComponent<FureyEngine::RenderComponent>(nullptr);
            AnimationComponent = AttachComponent<FureyEngine::AnimationComponent>(CurrentAnimation, Animation);
            CameraComponent = AttachComponent<FureyEngine::CameraComponent>();
            if (Collides) {
                BrushComponent = AttachComponent<FureyEngine::BrushComponent>(AnimationComponent);
            }
            MovementComponent = AttachComponent<FureyEngine::MovementComponent>();
            if (Physics) {
                PhysicsComponent = AttachComponent<FureyEngine::PhysicsComponent>(BrushComponent);
            }
        }
        return {this};
    }
}
