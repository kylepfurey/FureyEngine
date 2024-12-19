// .cpp
// Template Actor Class Script
// by Kyle Furey

#include "ExampleActor.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Constructs a new actor with a name.
    ExampleActor::ExampleActor(const std::string &Name, const bool &Active)
        : Actor(Name, Active) {
    }

    // New constructors here

    // EVENTS

    // Automatically called before each actor has already called Start().
    void ExampleActor::Spawn() {
        // Calls the base class's function
        Actor::Spawn();

        // Bind set active to the world's set active function
        GetWorld()->OnSetActive.Bind(std::bind(ExampleActor::SetActive, this, std::placeholders::_1));

        // New code here
    }

    // Automatically called when this actor is first created.
    void ExampleActor::Start() {
        // Calls the base class's function
        Actor::Start();

        // New code here
    }

    // Automatically called each world tick.
    void ExampleActor::Tick(const double &DeltaTime) {
        // Calls the base class's function
        Actor::Tick(DeltaTime);

        // New code here
    }

    // Automatically called after each actor has already called Tick().
    void ExampleActor::LateTick(const double &DeltaTime) {
        // Calls the base class's function
        Actor::LateTick(DeltaTime);

        // New code here
    }

    // Automatically called after this actor is destroyed.
    void ExampleActor::Destroy() {
        // Calls the base class's function
        Actor::Destroy();

        // Unbind set active from the world's set active function
        if (BeginTick) {
            GetWorld()->OnSetActive.Unbind(std::bind(ExampleActor::SetActive, this, std::placeholders::_1));
        }

        // New code here
    }

    // ACTIVITY

    // Sets whether this actor is active.
    // This can be overridden to control custom functionality.
    void ExampleActor::SetActive(const bool &Active) {
        // Calls the base class's function
        Actor::SetActive(Active);

        // New code here
    }
}
