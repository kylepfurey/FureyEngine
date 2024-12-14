// .h
// Scriptable Actor Class Script
// by Kyle Furey

#pragma once
#include "../Math/Math.h"
#include "../Components/Component.h"

namespace FureyEngine {
    // Forward declaration of world.
    class World;

    /**
     * Represents a single entity that can be rendered and run code within its world.<br/>
     * Inherit from this class to override scriptable methods and add custom logic.<br/>
     * NOTE: Actors should be constructed within their worlds using World->SpawnActor(...).
     */
    class Actor : public std::enable_shared_from_this<Actor> {
        // WORLD

        /** This actor's owning world. */
        Reference<World> MyWorld;

        /** The time point this actor was spawned into a world. */
        const std::chrono::high_resolution_clock::time_point SpawnTimePoint = std::chrono::high_resolution_clock::now();

        // ACTIVITY

        /** Whether this actor is active and should tick. */
        bool Active = true;

        // TOTAL ACTORS

        /** The total number of actors currently allocated. */
        static int TotalActors;

    public:
        // ACTOR SETTINGS

        /** This actor's transform. */
        Transform Transform = FureyEngine::Transform();

        /** The name of this actor. */
        std::string Name = "New Actor";

        /** A tag used to identify this actor. */
        std::string Tag = "NULL";

        /** Whether tick should be called over start. */
        bool BeginTick = false;

        // COMPONENTS

        /** This actor's components. */
        std::list<std::shared_ptr<Component> > Components;

        // EVENTS

        /**
         * Events to call before start events are called.<br/>
         * The parameter of this delegate is a pointer to this actor.
         */
        Delegate<Actor *> OnSpawn;

        /**
         * Events to call the first tick after this actor is first created.<br/>
         * The parameter of this delegate is a pointer to this actor.
         */
        Delegate<Actor *> OnStart;

        /**
         * Events to call when this actor is updated by its world. <br/>
         * The parameters of this delegate is a pointer to this actor and the world's current delta time.
         */
        Delegate<Actor *, const double &> OnTick;

        /**
         * Events to call after tick events are called.<br/>
         * The parameters of this delegate is a pointer to this actor and the world's current delta time.
         */
        Delegate<Actor *, const double &> OnLateTick;

        /**
         * Events to call when this actor is destroyed.<br/>
         * The parameter of this delegate is a pointer to this actor.
         */
        Delegate<Actor *> OnDestroy;

        /**
         * Events to call when SetActive() is called to sync activity.<br/>
         * The parameter of this delegate is a bool representing whether this actor is now active.
         */
        Delegate<const bool &> OnSetActive;

        // CONSTRUCTORS

        /** Constructs a new actor with a name. */
        explicit Actor(std::string Name = "New Actor", const bool &Active = true);

        /** Delete copy constructor. */
        Actor(const Actor &Copied) = delete;

        /** Delete move constructor. */
        Actor(Actor &&Moved) = delete;

        // ASSIGNMENT OPERATORS

        /** Delete copy constructor. */
        Actor &operator=(const Actor &Copied) = delete;

        /** Delete move constructor. */
        Actor &operator=(Actor &&Moved) = delete;

        // DESTRUCTOR

        /** Properly destroys this actor. */
        virtual ~Actor();

        // WORLD

        /**
         * Sets the world this actor resides in.<br/>
         * NOTE: If the actor was spawned using World->SpawnActor(...), this is automatically called.
         */
        void SetWorld(const std::shared_ptr<World> &OwningWorld);

        /** Returns this actor's owning world. */
        [[nodiscard]] Reference<World> GetWorld() const;

        /** Returns this actor's spawn time. */
        [[nodiscard]] std::chrono::high_resolution_clock::time_point SpawnTime() const;

        // COMPONENTS

        /** Attaches a new component of the given class to this actor. */
        template<typename ComponentType, typename... ArgumentTypes>
        Reference<ComponentType> AttachComponent(ArgumentTypes... Arguments) {
            const auto Component = new ComponentType(Arguments...);
            Components.emplace_back(Component);
            Component->SetActor(shared_from_this());
            return std::dynamic_pointer_cast<ComponentType>(Components.back());
        }

        /** Removes the given component from this actor at the end of the tick, and returns whether it was successfully found. */
        template<typename ComponentType>
        bool RemoveComponent(const ComponentType *RemovedComponent) {
            if (RemovedComponent == nullptr) {
                return false;
            }

            for (auto &Component: Components) {
                if (Component.get() == RemovedComponent) {
                    MyWorld->Cleanup.Bind([&] {
                        if (Component == nullptr) {
                            return;
                        }

                        // Call remove events
                        Component->Remove();
                        Component->OnRemove(Component.get());

                        // Delete this component
                        Components.remove(Component);
                    });
                    return true;
                }
            }
            return false;
        }

        /**
         * Returns the first component that matches the given name<br/>
         * or nullptr if it does not exist on this actor.
         */
        [[nodiscard]] Reference<Component> GetComponent(const std::string &Name) const;

        /**
         * Returns the first component of the given type<br/>
         * or nullptr if it does not exist on this actor.
         */
        template<typename ComponentType>
        [[nodiscard]] Reference<ComponentType> GetComponent() const {
            for (const auto &Component: Components) {
                if (Component->Is<ComponentType>()) {
                    return std::dynamic_pointer_cast<ComponentType>(Component);
                }
            }
            return nullref;
        }

        /**
         * Returns the first component of the given type that matches the given name<br/>
         * or nullptr if it does not exist on this actor.
         */
        template<typename ComponentType>
        [[nodiscard]] Reference<ComponentType> GetComponent(const std::string &Name) const {
            for (const auto &Component: Components) {
                if (Component->Name == Name && Component->Is<ComponentType>()) {
                    return std::dynamic_pointer_cast<ComponentType>(Component);
                }
            }
            return nullref;
        }

        // TRANSFORM

        /** Returns this actor's world transform. */
        [[nodiscard]] FureyEngine::Transform GetTransform() const;

        /** Returns this actor's world position. */
        [[nodiscard]] glm::vec2 GetPosition() const;

        /** Returns this actor's world rotation. */
        [[nodiscard]] float GetRotation() const;

        /** Returns this actor's world scale. */
        [[nodiscard]] glm::vec2 GetScale() const;

        /** Sets this actor's world transform. */
        void SetTransform(const FureyEngine::Transform &Transform);

        /** Sets this actor's world position. */
        void SetPosition(const glm::vec2 &Position);

        /** Sets this actor's world rotation. */
        void SetRotation(const float &Rotation);

        /** Sets this actor's world scale. */
        void SetScale(const glm::vec2 &Scale);

        // EVENTS

        /** Automatically called before each actor has already called Start(). */
        virtual void Spawn();

        /** Automatically called when this actor is first created. */
        virtual void Start();

        /** Automatically called each world tick. */
        virtual void Tick(const double &DeltaTime);

        /** Automatically called after each actor has already called Tick(). */
        virtual void LateTick(const double &DeltaTime);

        /** Automatically called after this actor is destroyed. */
        virtual void Destroy();

        // CASTING

        /** Returns whether this actor is the given actor type. */
        template<typename ActorType>
        [[nodiscard]] bool Is() const {
            return typeid(*this).hash_code() == typeid(ActorType).hash_code();
        }

        /**
         * Casts this actor to the given actor type.<br/>
         * Returns nullptr if the cast failed.
         */
        template<typename NewActorType>
        [[nodiscard]] NewActorType *Cast() {
            return dynamic_cast<NewActorType *>(this);
        }

        // ACTIVITY

        /** Returns whether this actor is active. */
        [[nodiscard]] bool IsActive() const;

        /**
         * Sets whether this actor is active.<br/>
         * This can be overridden to control custom functionality.
         */
        virtual void SetActive(const bool &Active);
    };
}
