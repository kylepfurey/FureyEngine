// .cpp
// Sprite Actor Class Script
// by Kyle Furey

#include "SpriteActor.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Constructs a new sprite with a name.
    SpriteActor::SpriteActor(const std::string &Name, const bool &Active)
        : Actor(Name, Active) {
    }

    // SPRITE

    // Returns whether this sprite has been initialized yet.
    bool SpriteActor::IsInitialized() const {
        return Initialized;
    }

    // Initializes this sprite's components with an image.
    Reference<SpriteActor> SpriteActor::Initialize(Image *Image,
                                                   const bool &Collides, const bool &Physics) {
        if (!Initialized) {
            Initialized = true;
            RenderComponent = AttachComponent<FureyEngine::RenderComponent>(Image);
            if (Collides) {
                BrushComponent = AttachComponent<FureyEngine::BrushComponent>(RenderComponent);
            }
            if (Physics) {
                PhysicsComponent = AttachComponent<FureyEngine::PhysicsComponent>(BrushComponent);
            }
        }
        return {this};
    }

    // Initializes this sprite's components with an animation.
    Reference<SpriteActor> SpriteActor::Initialize(const std::string &CurrentAnimation, Animation *Animation,
                                                   const bool &Collides, const bool &Physics) {
        if (!Initialized) {
            Initialized = true;
            RenderComponent = AttachComponent<FureyEngine::RenderComponent>(nullptr);
            AnimationComponent = AttachComponent<FureyEngine::AnimationComponent>(CurrentAnimation, Animation);
            if (Collides) {
                BrushComponent = AttachComponent<FureyEngine::BrushComponent>(AnimationComponent);
            }
            if (Physics) {
                PhysicsComponent = AttachComponent<FureyEngine::PhysicsComponent>(BrushComponent);
            }
        }
        return {this};
    }
}
