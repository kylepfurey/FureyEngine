// .h
// Scriptable Component Class Script
// by Kyle Furey

#pragma once
#include "../Delegate/Delegate.h"
#include "../Event/Event.h"
#include "../Reference/Reference.h"

namespace FureyEngine {
    // Forward declaration of actor.
    class Actor;

    /**
     * Represents modular code that can be attached to an actor.<br/>
     * Inherit from this class to override scriptable methods and add custom logic.<br/>
     * NOTE: Components should be constructed within their actors using Actor->AttachComponent(...).
     */
    class Component : public std::enable_shared_from_this<Component> {
        // ACTOR

        /** This component's owning actor. */
        Reference<Actor> MyActor;

        /** The time point this component was attached to an actor. */
        const std::chrono::high_resolution_clock::time_point AttachTimePoint =
                std::chrono::high_resolution_clock::now();

        /** Whether this component is active and should tick. */
        bool Active = true;

        // TOTAL COMPONENTS

        /** The total number of components currently allocated. */
        static int TotalComponents;

    public:
        // COMPONENT SETTINGS

        /** The name of this component. */
        std::string Name = "New Component";

        /** A tag used to identify this component. */
        std::string Tag = "NULL";

        /** Whether tick should be called over start. */
        bool BeginTick = false;

        // EVENTS

        /**
         * Events to call before start events are called.<br/>
         * The parameter of this delegate is a pointer to this component.
         */
        Delegate<Component *> OnAttach;

        /**
         * Events to call the first tick after this component is first created.<br/>
         * The parameter of this delegate is a pointer to this component.
         */
        Delegate<Component *> OnStart;

        /**
         * Events to call when this actor is updated by its world. <br/>
         * The parameters of this delegate is a pointer to this component and the world's current delta time.
         */
        Delegate<Component *, const double &> OnTick;

        /**
         * Events to call after tick events are called.<br/>
         * The parameters of this delegate is a pointer to this component and the world's current delta time.
         */
        Delegate<Component *, const double &> OnLateTick;

        /**
         * Events to call when this component is removed.<br/>
         * The parameter of this delegate is a pointer to this component.
         */
        Delegate<Component *> OnRemove;

        /**
         * Events to call when SetActive() is called to sync activity.<br/>
         * The parameter of this delegate is a bool representing whether this component is now active.
         */
        Delegate<const bool &> OnSetActive;

        // CONSTRUCTORS

        /** Constructs a new component with a name. */
        explicit Component(std::string Name = "New Component", const bool &Active = true);

        /** Delete copy constructor. */
        Component(const Component &Copied) = delete;

        /** Delete move constructor. */
        Component(Component &&Moved) = delete;

        // ASSIGNMENT OPERATORS

        /** Delete copy constructor. */
        Component &operator=(const Component &Copied) = delete;

        /** Delete move constructor. */
        Component &operator=(Component &&Moved) = delete;

        // DESTRUCTOR

        /** Properly destroys this component. */
        virtual ~Component();

        // ACTOR

        /**
         * Sets the actor this component is attached to.<br/>
         * NOTE: If the component was attached using Actor->AttachComponent(...), this is automatically called.
         */
        void SetActor(const std::shared_ptr<Actor> &OwningActor);

        /** Returns this component's owning actor. */
        [[nodiscard]] Reference<Actor> GetActor() const;

        /** Returns this component's attach time. */
        [[nodiscard]] std::chrono::high_resolution_clock::time_point AttachTime() const;

        // EVENTS

        /** Automatically called before each component has already called Start(). */
        virtual void Attach();

        /** Automatically called when this component is first created. */
        virtual void Start();

        /** Automatically called each world tick. */
        virtual void Tick(const double &DeltaTime);

        /** Automatically called after each component has already called Tick(). */
        virtual void LateTick(const double &DeltaTime);

        /** Automatically called after this component is removed. */
        virtual void Remove();

        // CASTING

        /** Returns whether this component is the given component type. */
        template<typename ComponentType>
        [[nodiscard]] bool Is() const {
            return typeid(*this).hash_code() == typeid(ComponentType).hash_code();
        }

        /**
         * Casts this component to the given component type.<br/>
         * Returns nullptr if the cast failed.
         */
        template<typename NewComponentType>
        [[nodiscard]] NewComponentType *Cast() {
            return dynamic_cast<NewComponentType *>(this);
        }

        // ACTIVITY

        /** Returns whether this component is active. */
        [[nodiscard]] bool IsActive() const;

        /**
         * Sets whether this component is active.<br/>
         * This can be overridden to control custom functionality.
         */
        virtual void SetActive(const bool &Active);
    };
}
