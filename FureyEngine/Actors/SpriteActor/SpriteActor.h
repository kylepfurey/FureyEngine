// .h
// Sprite Actor Class Script
// by Kyle Furey

#pragma once
#include "../../Components/PhysicsComponent/PhysicsComponent.h"

namespace FureyEngine {
    /** An actor with a set of components that allow it to be easily rendered with an image or animation. */
    class SpriteActor : public Actor {
    protected:
        // SPRITE

        /** Whether this sprite has been initialized yet. */
        bool Initialized = false;

    public:
        // SPRITE

        /**
         * A reference to the render component of this sprite, used to render this actor to the screen.<br/>
         * This is automatically attached when Initialize() is called.
         */
        Reference<RenderComponent> RenderComponent = nullref;

        /**
         * A reference to the animation component of this sprite, used to animate this actor's texture.<br/>
         * This is automatically attached when Initialize() is called with an animation.
         */
        Reference<AnimationComponent> AnimationComponent = nullref;

        /**
         * A reference to the brush component of this sprite, used for the actor's collision with the world.<br/>
         * This is automatically attached when Initialize() is called if Collides is set to true.
         */
        Reference<BrushComponent> BrushComponent = nullref;

        /**
         * A reference to the physics component of this sprite, used to give the actor physics.<br/>
         * This is automatically attached when Initialize() is called if Physics is set to true.
         */
        Reference<PhysicsComponent> PhysicsComponent = nullref;

        // CONSTRUCTORS

        /** Constructs a new sprite with a name. */
        explicit SpriteActor(const std::string &Name = "Sprite", const bool &Active = true);

        // SPRITE

        /** Returns whether this sprite has been initialized yet. */
        [[nodiscard]] virtual bool IsInitialized() const;

        /** Initializes this sprite's components with an image. */
        virtual Reference<SpriteActor> Initialize(Image *Image,
                                                  const bool &Collides = true, const bool &Physics = false);

        /** Initializes this sprite's components with an animation. */
        virtual Reference<SpriteActor> Initialize(const std::string &CurrentAnimation, Animation *Animation,
                                                  const bool &Collides = true, const bool &Physics = false);
    };
}
