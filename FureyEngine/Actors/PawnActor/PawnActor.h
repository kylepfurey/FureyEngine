// .h
// Player Pawn Actor Class Script
// by Kyle Furey

#pragma once
#include "../../Components/CameraComponent/CameraComponent.h"
#include "../../Components/MovementComponent/MovementComponent.h"
#include "../../Components/PhysicsComponent/PhysicsComponent.h"

namespace FureyEngine {
    /** An actor with a set of components that allow it to be easily used as a player. */
    class PawnActor : public Actor {
    protected:
        // PAWN

        /** Whether this player pawn has been initialized yet. */
        bool Initialized = false;

    public:
        // PAWN

        /**
         * A reference to the render component of this player pawn, used to render this player to the screen.<br/>
         * This is automatically attached when Initialize() is called.
         */
        Reference<RenderComponent> RenderComponent = nullref;

        /**
         * A reference to the animation component of this player pawn, used to animate this player's texture.<br/>
         * This is automatically attached when Initialize() is called with an animation.
         */
        Reference<AnimationComponent> AnimationComponent = nullref;

        /**
         * A reference to the camera component of this player pawn, used to keep the camera following the player.<br/>
         * This is automatically attached when Initialize() is called.
         */
        Reference<CameraComponent> CameraComponent = nullref;

        /**
         * A reference to the brush component of this player pawn, used for the player's collision with the world.<br/>
         * This is automatically attached when Initialize() is called if Collides is set to true.
         */
        Reference<BrushComponent> BrushComponent = nullref;

        /**
         * A reference to the movement component of this player pawn, used for the player's movement.<br/>
         * This is automatically attached when Initialize() is called.
         */
        Reference<MovementComponent> MovementComponent = nullref;

        /**
         * A reference to the physics component of this player pawn, used to give the player physics.<br/>
         * This is automatically attached when Initialize() is called if Physics is set to true.
         */
        Reference<PhysicsComponent> PhysicsComponent = nullref;

        // CONSTRUCTORS

        /** Constructs a new player pawn with a name. */
        explicit PawnActor(const std::string &Name = "Player", const bool &Active = true);

        // PAWN

        /** Returns whether this player pawn has been initialized yet. */
        [[nodiscard]] virtual bool IsInitialized() const;

        /** Initializes this player pawn's components with an image. */
        virtual Reference<PawnActor> Initialize(Image *Image,
                                                const bool &Collides = true, const bool &Physics = false);

        /** Initializes this player pawn's components with an animation. */
        virtual Reference<PawnActor> Initialize(const std::string &CurrentAnimation, Animation *Animation,
                                                const bool &Collides = true, const bool &Physics = false);
    };
}
