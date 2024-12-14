// .cpp
// Scriptable Component Class Script
// by Kyle Furey

#include "Component.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Constructs a new component with a name.
    Component::Component(std::string Name, const bool &Active)
        : Active(Active), Name(std::move(Name)) {
    }

    // DESTRUCTOR

    // Properly destroys this component.
    Component::~Component() {
        --TotalComponents;
    }

    // ACTOR

    // Sets the actor this component is attached to.
    // NOTE: If the component was attached using Actor->AttachComponent(...), this is automatically called
    void Component::SetActor(const std::shared_ptr<Actor> &OwningActor) {
        if (!MyActor) {
            MyActor = OwningActor;
            ++TotalComponents;
        } else {
            throw std::runtime_error(
                std::string("ERROR: Cannot set the actor of component \"") + Name + "\" more than once!\n"
                "Components automatically set their actor with Actor->AttachComponent(...)!");
        }
    }

    // Returns this component's owning actor.
    Reference<Actor> Component::GetActor() const {
        return MyActor;
    }

    // Returns this component's attach time.
    std::chrono::high_resolution_clock::time_point Component::AttachTime() const {
        return AttachTimePoint;
    }

    // EVENTS

    // Automatically called before each component has already called Start().
    void Component::Attach() {
        // std::cout << "Component \"" << Name << "\" was attached." << std::endl;
    }

    // Automatically called when this component is first created.
    void Component::Start() {
        // std::cout << "Component \"" << Name << "\" started." << std::endl;
    }

    // Automatically called each world tick.
    void Component::Tick(const double &DeltaTime) {
        // std::cout << "Component \"" << Name << "\" ticked." << std::endl;
    }

    // Automatically called after each component has already called Tick().
    void Component::LateTick(const double &DeltaTime) {
        // std::cout << "Component \"" << Name << "\" ticked again." << std::endl;
    }

    // Automatically called after this component is removed.
    void Component::Remove() {
        // std::cout << "Component \"" << Name << "\" was removed." << std::endl;
    }

    // ACTIVITY

    // Returns whether this component is active.
    bool Component::IsActive() const {
        return Active;
    }

    // Sets whether this component is active.
    // This can be overridden to control custom functionality.
    void Component::SetActive(const bool &Active) {
        this->Active = Active;
        OnSetActive(Active);
    }

    // STATIC VARIABLE INITIALIZATION

    // The total number of components currently allocated.
    int Component::TotalComponents;
}
