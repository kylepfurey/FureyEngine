// .cpp
// Template World Class Script
// by Kyle Furey

#include "ExampleWorld.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Constructs a new world with a name.
    ExampleWorld::ExampleWorld(const std::string &Name, const bool &Active)
        : World(Name, Active) {
    }

    // New constructors here

    // EVENTS

    // Automatically called to create new actors.
    void ExampleWorld::Load() {
        // Calls the base class's function
        World::Load();

        // New code here
    }

    // Automatically called after the creation of actors.
    void ExampleWorld::Start() {
        // Calls the base class's function
        World::Start();

        // New code here
    }

    // Automatically called each tick.
    void ExampleWorld::Tick(const double &DeltaTime) {
        // Calls the base class's function
        World::Tick(DeltaTime);

        // New code here
    }

    // Automatically called after each actor and component has already called Tick().
    void ExampleWorld::LateTick(const double &DeltaTime) {
        // Calls the base class's function
        World::LateTick(DeltaTime);

        // New code here
    }

    // Automatically called after this world is unloaded.
    void ExampleWorld::Unload() {
        // Calls the base class's function
        World::Unload();

        // New code here
    }

    // ACTIVITY

    // Sets whether this world is active.
    // This can be overridden to control custom functionality.
    void ExampleWorld::SetActive(const bool &Active) {
        // Calls the base class's function
        World::SetActive(Active);

        // New code here
    }
}
