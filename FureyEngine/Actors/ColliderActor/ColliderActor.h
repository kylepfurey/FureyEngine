// .h
// Collider Actor Class Script
// by Kyle Furey

#pragma once
#include "../../Components/BrushComponent/BrushComponent.h"

namespace FureyEngine {
    /** An actor with a brush component used for collisions. */
    class ColliderActor : public Actor {
    protected:
        // COLLIDER

        /** Whether this collider has been initialized yet. */
        bool Initialized = false;

    public:
        // COLLIDER

        /** A reference to the brush component of this collider, used for collision with the world. */
        Reference<BrushComponent> BrushComponent = nullref;

        // CONSTRUCTORS

        /** Constructs a new collider with a name. */
        explicit ColliderActor(const std::string &Name = "Collider", const bool &Active = true);

        // COLLIDER

        /** Returns whether this collider has been initialized yet. */
        [[nodiscard]] virtual bool IsInitialized() const;

        /** Initializes this collider with the given transform. */
        virtual Reference<ColliderActor> Initialize(const FureyEngine::Transform &Bounds,
                                                    const std::string &Layer = "All");

        /** Initializes this collider with the given size. */
        virtual Reference<ColliderActor> Initialize(const glm::vec2 &Size,
                                                    const std::string &Layer = "All");

        /** Initializes this collider with the given render component. */
        virtual Reference<ColliderActor> Initialize(const Reference<RenderComponent> &RenderComponent,
                                                    const std::string &Layer = "All");

        /** Initializes this collider with the given animation component. */
        virtual Reference<ColliderActor> Initialize(const Reference<AnimationComponent> &AnimationComponent,
                                                    const std::string &Layer = "All");
    };
}
