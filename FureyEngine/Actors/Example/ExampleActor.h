// .h
// Template Actor Class Script
// by Kyle Furey

#pragma once
#include "../../Engine/Engine.h"

namespace FureyEngine {
    /** An example actor script that can be copied to create new actors. */
    class ExampleActor : public Actor {
    protected:
        // VARIABLES

        // New variables here

    public:
        // CONSTRUCTORS

        /** Constructs a new actor with a name. */
        explicit ExampleActor(const std::string &Name = "New Actor", const bool &Active = true);

        // New constructors here

        // EVENTS

        /** Automatically called before each actor has already called Start(). */
        void Spawn() override;

        /** Automatically called when this actor is first created. */
        void Start() override;

        /** Automatically called each world tick. */
        void Tick(const double &DeltaTime) override;

        /** Automatically called after each actor has already called Tick(). */
        void LateTick(const double &DeltaTime) override;

        /** Automatically called after this actor is destroyed. */
        void Destroy() override;

        // ACTIVITY

        /**
         * Sets whether this actor is active.<br/>
         * This can be overridden to control custom functionality.
         */
        void SetActive(const bool &Active) override;

        // FUNCTIONS

        // New functions here
    };
}
