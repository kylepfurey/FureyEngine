// .cpp
// Template Component Class Script
// by Kyle Furey

#include "ExampleComponent.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Constructs a new component with a name.
    ExampleComponent::ExampleComponent(const std::string &Name, const bool &Active)
        : Component(Name, Active) {
    }

    // New constructors here

    // EVENTS

    // Automatically called before each component has already called Start().
    void ExampleComponent::Attach() {
        // Calls the base class's function
        Component::Attach();

        // Bind set active to the actor's set active function
        GetActor()->OnSetActive.Bind(std::bind(ExampleComponent::SetActive, this, std::placeholders::_1));

        // New code here
    }

    // Automatically called when this component is first created.
    void ExampleComponent::Start() {
        // Calls the base class's function
        Component::Start();

        // New code here
    }

    // Automatically called each world tick.
    void ExampleComponent::Tick(const double &DeltaTime) {
        // Calls the base class's function
        Component::Tick(DeltaTime);

        // New code here
    }

    // Automatically called after each component has already called Tick().
    void ExampleComponent::LateTick(const double &DeltaTime) {
        // Calls the base class's function
        Component::LateTick(DeltaTime);

        // New code here
    }

    // Automatically called after this component is removed.
    void ExampleComponent::Remove() {
        // Calls the base class's function
        Component::Remove();

        // Unbind set active from the actor's set active function
        if (BeginTick) {
            GetActor()->OnSetActive.Unbind(std::bind(ExampleComponent::SetActive, this, std::placeholders::_1));
        }

        // New code here
    }

    // ACTIVITY

    // Sets whether this component is active.
    // This can be overridden to control custom functionality.
    void ExampleComponent::SetActive(const bool &Active) {
        // Calls the base class's function
        Component::SetActive(Active);

        // New code here
    }
}
