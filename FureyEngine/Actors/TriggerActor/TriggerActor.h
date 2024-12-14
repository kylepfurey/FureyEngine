// .h
// Trigger Box Actor Class Script
// by Kyle Furey

#pragma once
#include "../ColliderActor/ColliderActor.h"

namespace FureyEngine {
    /** An actor with a brush component used for triggers. */
    class TriggerActor : public ColliderActor {
    protected:
        // TRIGGER

        /** A function used to dispatch trigger collision events. */
        virtual void OnCollision(const FureyEngine::BrushComponent *MyBrushComponent,
                                 FureyEngine::BrushComponent *OtherBrushComponent);

    public:
        // TRIGGER

        /**
         * A delegate that is fired when a brush component enters this trigger.<br/>
         * The first parameter is a pointer to this trigger.<br/>
         * The second parameter is a pointer to the colliding brush component.
         */
        Delegate<TriggerActor *, FureyEngine::BrushComponent *> OnTriggerEnter;

        /**
         * A delegate that is fired when a brush component is no longer inside this trigger.<br/>
         * The first parameter is a pointer to this trigger.<br/>
         * The second parameter is a pointer to the leaving brush component.
         */
        Delegate<TriggerActor *, FureyEngine::BrushComponent *> OnTriggerExit;

        // CONSTRUCTORS

        /** Constructs a new trigger box with a name. */
        explicit TriggerActor(const std::string &Name = "Trigger", const bool &Active = true);

        // TRIGGER

        /** Initializes this trigger box with the given transform. */
        Reference<ColliderActor> Initialize(const FureyEngine::Transform &Bounds,
                                            const std::string &Layer = "All") override;

        /** Initializes this trigger box with the given size. */
        Reference<ColliderActor> Initialize(const glm::vec2 &Size,
                                            const std::string &Layer = "All") override;

        /** Initializes this trigger box with the given render component. */
        Reference<ColliderActor> Initialize(const Reference<RenderComponent> &RenderComponent,
                                            const std::string &Layer = "All") override;

        /** Initializes this trigger box with the given animation component. */
        Reference<ColliderActor> Initialize(const Reference<AnimationComponent> &AnimationComponent,
                                            const std::string &Layer = "All") override;
    };
}
