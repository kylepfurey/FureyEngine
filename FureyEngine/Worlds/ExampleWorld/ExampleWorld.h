// .h
// Template World Class Script
// by Kyle Furey

#pragma once
#include "../../Engine/Engine.h"

namespace FureyEngine {
    /** An example world script that can be copied to create new worlds. */
    class ExampleWorld : public World {
    protected:
        // VARIABLES

        // New variables here

    public:
        // CONSTRUCTORS

        /** Constructs a new world with a name. */
        explicit ExampleWorld(const std::string &Name = "New World", const bool &Active = true);

        // New constructors here

        // EVENTS

        /** Automatically called to create new actors. */
        void Load() override;

        /** Automatically called after the creation of actors. */
        void Start() override;

        /** Automatically called each tick. */
        void Tick(const double &DeltaTime) override;

        /** Automatically called after each actor and component has already called Tick(). */
        void LateTick(const double &DeltaTime) override;

        /** Automatically called after this world is unloaded. */
        void Unload() override;

        // ACTIVITY

        /**
         * Sets whether this world is active.<br/>
         * This can be overridden to control custom functionality.
         */
        void SetActive(const bool &Active) override;

        // FUNCTIONS

        // New functions here
    };
}
