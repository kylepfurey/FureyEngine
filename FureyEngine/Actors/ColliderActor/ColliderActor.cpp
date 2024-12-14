// .cpp
// Collider Actor Class Script
// by Kyle Furey

#include "ColliderActor.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Constructs a new collider with a name.
    ColliderActor::ColliderActor(const std::string &Name, const bool &Active)
        : Actor(Name, Active) {
    }

    // COLLIDER

    // Returns whether this collider has been initialized yet.
    bool ColliderActor::IsInitialized() const {
        return Initialized;
    }

    // Initializes this collider with the given transform.
    Reference<ColliderActor> ColliderActor::Initialize(const FureyEngine::Transform &Bounds,
                                                       const std::string &Layer) {
        if (!Initialized) {
            Initialized = true;
            BrushComponent = AttachComponent<FureyEngine::BrushComponent>(Bounds, false, Layer);
        }
        return {this};
    }

    // Initializes this collider with the given size.
    Reference<ColliderActor> ColliderActor::Initialize(const glm::vec2 &Size,
                                                       const std::string &Layer) {
        if (!Initialized) {
            Initialized = true;
            BrushComponent = AttachComponent<FureyEngine::BrushComponent>(
                FureyEngine::Transform({0, 0}, 0, Size), false, Layer);
        }
        return {this};
    }

    // Initializes this collider with the given render component.
    Reference<ColliderActor> ColliderActor::Initialize(const Reference<RenderComponent> &RenderComponent,
                                                       const std::string &Layer) {
        if (!Initialized) {
            Initialized = true;
            BrushComponent = AttachComponent<FureyEngine::BrushComponent>(RenderComponent, false, Layer);
        }
        return {this};
    }

    // Initializes this collider with the given animation component.
    Reference<ColliderActor> ColliderActor::Initialize(const Reference<AnimationComponent> &AnimationComponent,
                                                       const std::string &Layer) {
        if (!Initialized) {
            Initialized = true;
            BrushComponent = AttachComponent<FureyEngine::BrushComponent>(AnimationComponent, false, Layer);
        }
        return {this};
    }
}
