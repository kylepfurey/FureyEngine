// .cpp
// Scriptable Actor Class Script
// by Kyle Furey

#include "Actor.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Constructs a new actor with a name.
    Actor::Actor(std::string Name, const bool &Active)
        : Active(Active), Name(std::move(Name)) {
    }

    // DESTRUCTOR

    // Properly destroys this actor.
    Actor::~Actor() {
        --TotalActors;
    }

    // WORLD

    // Sets the world this actor resides in.
    // NOTE: If the actor was spawned using World->SpawnActor(...), this is automatically called.
    void Actor::SetWorld(const std::shared_ptr<World> &OwningWorld) {
        if (!MyWorld) {
            MyWorld = OwningWorld;
            ++TotalActors;
        } else {
            throw std::runtime_error(
                std::string("ERROR: Cannot set the world of actor \"") + Name + "\" more than once!\n"
                "Actors automatically set their world with World->SpawnActor(...)!");
        }
    }

    // Returns this actor's owning world.
    Reference<World> Actor::GetWorld() const {
        return MyWorld;
    }

    // Returns this actor's spawn time.
    std::chrono::high_resolution_clock::time_point Actor::SpawnTime() const {
        return SpawnTimePoint;
    }

    // COMPONENTS

    // Returns the first component that matches the given name
    // or nullptr if it does not exist on this actor.
    Reference<Component> Actor::GetComponent(const std::string &Name) const {
        for (auto &Component: Components) {
            if (Component->Name == Name) {
                return Component;
            }
        }
        return nullref;
    }

    // TRANSFORM

    // Returns this actor's world transform.
    Transform Actor::GetTransform() const {
        return Transform;
    }

    // Returns this actor's world position.
    glm::vec2 Actor::GetPosition() const {
        return Transform.Position;
    }

    // Returns this actor's world rotation.
    float Actor::GetRotation() const {
        return Transform.Rotation;
    }

    // Returns this actor's world scale.
    glm::vec2 Actor::GetScale() const {
        return Transform.Scale;
    }

    // Sets this actor's world transform.
    void Actor::SetTransform(const FureyEngine::Transform &Transform) {
        this->Transform = Transform;
    }

    // Sets this actor's world position.
    void Actor::SetPosition(const glm::vec2 &Position) {
        Transform.Position = Position;
    }

    // Sets this actor's world rotation.
    void Actor::SetRotation(const float &Rotation) {
        Transform.Rotation = Rotation;
    }

    // Sets this actor's world scale.
    void Actor::SetScale(const glm::vec2 &Scale) {
        Transform.Scale = Scale;
    }

    // EVENTS

    // Automatically called before each actor has already called Start().
    void Actor::Spawn() {
        // std::cout << "Actor \"" << Name << "\" was initialized." << std::endl;
    }

    // Automatically called when this actor is first created.
    void Actor::Start() {
        // std::cout << "Actor \"" << Name << "\" started." << std::endl;
    }

    // Automatically called each world tick.
    void Actor::Tick(const double &DeltaTime) {
        // std::cout << "Actor \"" << Name << "\" ticked." << std::endl;
    }

    // Automatically called after each actor has already called Tick().
    void Actor::LateTick(const double &DeltaTime) {
        // std::cout << "Actor \"" << Name << "\" ticked again." << std::endl;
    }

    // Automatically called after this actor is destroyed.
    void Actor::Destroy() {
        // std::cout << "Actor \"" << Name << "\" was destroyed." << std::endl;
    }

    // ACTIVITY

    // Returns whether this actor is active.
    bool Actor::IsActive() const {
        return Active;
    }

    // Sets whether this actor is active.
    // This can be overridden to control custom functionality.
    void Actor::SetActive(const bool &Active) {
        this->Active = Active;
        OnSetActive(Active);
    }

    // STATIC VARIABLE INITIALIZATION

    // The total number of actors currently allocated.
    int Actor::TotalActors;
}
