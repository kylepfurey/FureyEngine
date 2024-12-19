// .h
// Scriptable World Class Script
// by Kyle Furey

#pragma once
#include "../Actors/Actor.h"

namespace FureyEngine {
    /**
     * Manages a collection of actors and components that can run code each game tick when active.<br/>
     * Inherit from this class to create new custom worlds each with their own actors.
     */
    class World : public std::enable_shared_from_this<World> {
        // TIME

        /** The time point of this world when it was loaded. */
        const std::chrono::high_resolution_clock::time_point LoadTimePoint = std::chrono::high_resolution_clock::now();

        /** The current time point of this world. */
        std::chrono::high_resolution_clock::time_point CurrentTimePoint = std::chrono::high_resolution_clock::now();

        /** The time in seconds since last tick. */
        double CurrentDeltaTime = 0.0000001;

        /** The total number of ticks called of this world. */
        unsigned long long TotalTicks = 0;

        /** An event that is automatically invoked after a specific time. */
        struct TimedEvent final {
            // TIME

            /** The remaining time before invoking this function. */
            double Delay = 0;

            // EVENT

            /** The one time event to invoke after the delay reaches 0. */
            std::function<void()> Event = nullptr;

            // INVOKED

            /** Whether this time event has been invoked yet. */
            bool Invoked = false;
        };

        /** Events to call when they are scheduled. */
        std::list<TimedEvent> ScheduledEvents;

        // ACTIVITY

        /** Whether this world is active and should tick. */
        bool Active = true;

        // TOTAL WORLDS

        /** The total number of worlds currently allocated. */
        static int TotalWorlds;

    public:
        // WORLD SETTINGS

        /** The name of this world. */
        const std::string Name;

        /** A tag used to identify this world. */
        std::string Tag = "NULL";

        /** Whether tick should be called over start. */
        bool BeginTick = false;

        // TIME

        /** The scale applied to delta time in this world. */
        double TimeScale = 1;

        // ACTORS

        /** Each actor instance currently in this world. */
        std::list<std::shared_ptr<Actor> > Actors;

        // EVENTS

        /**
         * Events to call before spawning each actor in this world.<br/>
         * The parameter of this delegate is a pointer to this world.
         */
        Delegate<World *> OnLoad;

        /**
         * Events to call after each actor has spawned in this world.<br/>
         * The parameter of this delegate is a pointer to this world.
         */
        Delegate<World *> OnStart;

        /**
         * Events to call before each actor's tick call in this world.<br/>
         * The parameters of this delegate is a pointer to this world and its current delta time.
         */
        Delegate<World *, const double &> OnTick;

        /**
         * Events to call after each actor's tick call in this world.<br/>
         * The parameters of this delegate is a pointer to this world and its current delta time.
         */
        Delegate<World *, const double &> OnLateTick;

        /**
         * Events to call before each actor is destroyed in this world.<br/>
         * The parameter of this delegate is a pointer to this world.
         */
        Delegate<World *> OnUnload;

        /**
         * Events to call when SetActive() is called to sync activity.<br/>
         * The parameter of this delegate is a bool representing whether this world is now active.
         */
        Delegate<const bool &> OnSetActive;

        // CLEANUP

        /**
         * Events to call at the very end of a world update.<br/>
         * This is cleared after each Update() call.
         */
        Event Cleanup;

        // CONSTRUCTORS

        /** Constructs a new world with a name. */
        explicit World(std::string Name = "New World", const bool &Active = true);

        /** Delete copy constructor. */
        World(const World &Copied) = delete;

        /** Delete move constructor. */
        World(World &&Moved) = delete;

        // ASSIGNMENT OPERATORS

        /** Delete copy constructor. */
        World &operator=(const World &Copied) = delete;

        /** Delete move constructor. */
        World &operator=(World &&Moved) = delete;

        // DESTRUCTOR

        /** Properly destroys this world. */
        virtual ~World();

        // TIME

        /** Returns the time point this world was created. */
        [[nodiscard]] std::chrono::high_resolution_clock::time_point LoadTime() const;

        /** Returns the current time point as of this world's tick. */
        [[nodiscard]] std::chrono::high_resolution_clock::time_point CurrentTime() const;

        /** The time in seconds since this world was last ticked. */
        [[nodiscard]] double DeltaTime() const;

        /** Returns the elapsed time (in seconds) since this world was created. */
        [[nodiscard]] double ElapsedTime() const;

        /** Returns the total number of times Tick() has been called for this world. */
        [[nodiscard]] unsigned long long TickCount() const;

        /**
         * Invokes the given function after the given number of seconds.<br/>
         * Returns the scheduled event.
         */
        TimedEvent *InvokeAfterSeconds(double Delay,
                                       const std::function<void()> &Function);

        /**
         * Invokes the given function after the given number of seconds for the given number of loops.<br/>
         * If loops is equal to -1, it will loop forever.<br/>
         * Returns the scheduled event.
         */
        TimedEvent *RepeatAfterSeconds(double Delay,
                                       const int &Loops,
                                       const std::function<void()> &Function);

        /**
         * Invokes the given function after the given number of seconds for the given number of loops.<br/>
         * This function will loop forever unless cancelled.<br/>
         * Returns the scheduled event.
         */
        TimedEvent *RepeatAfterSeconds(const double &Delay,
                                       const std::function<void()> &Function);

        /**
         * Invokes the given function the following tick.<br/>
         * Returns the scheduled event.
         */
        TimedEvent *InvokeNextTick(const std::function<void()> &Function);

        /**
         * Attempt to unschedule the given function from the scheduled events.<br/>
         * Returns if the cancellation was successful.
         */
        bool CancelInvoke(const std::function<void()> &Function);

        // ACTORS

        /**
         * Updates the current state of this world and its actors.<br/>
         * Call this in each update loop.
         */
        void Update();

        /** Spawns a new actor of the given class in this world. */
        template<typename ActorType, typename... ArgumentTypes>
        Reference<ActorType> SpawnActor(const Transform &Transform, ArgumentTypes... Arguments) {
            const auto Actor = new ActorType(Arguments...);
            Actors.emplace_back(Actor);
            Actor->SetWorld(shared_from_this());
            Actor->Transform = Transform;
            return std::dynamic_pointer_cast<ActorType>(Actors.back());
        }

        /** Destroys the given actor at the end of the tick, and returns whether it was successfully found. */
        template<typename ActorType>
        bool DestroyActor(const ActorType *DestroyedActor) {
            if (DestroyedActor == nullptr) {
                return false;
            }

            for (auto &Actor: Actors) {
                if (Actor.get() == DestroyedActor) {
                    // Remove all of this actor's components
                    for (const auto &Component: Actor->Components) {
                        Actor->RemoveComponent(Component.get());
                    }

                    Cleanup.Bind([&] {
                        if (Actor == nullptr) {
                            return;
                        }

                        // Call destroy events
                        Actor->Destroy();
                        Actor->OnDestroy(Actor.get());

                        // Delete this actor
                        Actors.remove(Actor);
                    });
                    return true;
                }
            }
            return false;
        }

        /**
         * Returns the first actor that matches the given name<br/>
         * or nullptr if it does not exist in this world.
         */
        [[nodiscard]] Reference<Actor> GetActor(const std::string &Name) const;

        /**
         * Returns the first actor of the given type<br/>
         * or nullptr if it does not exist in this world.
         */
        template<typename ActorType>
        [[nodiscard]] Reference<ActorType> GetActor() const {
            for (const auto &Actor: Actors) {
                if (Actor->Is<ActorType>()) {
                    return std::dynamic_pointer_cast<ActorType>(Actor);
                }
            }
            return nullref;
        }

        /**
         * Returns the first actor of the given type that matches the given name<br/>
         * or nullptr if it does not exist in this world. 
         */
        template<typename ActorType>
        [[nodiscard]] Reference<ActorType> GetActor(const std::string &Name) const {
            for (const auto &Actor: Actors) {
                if (Actor->Name == Name && Actor->Is<ActorType>()) {
                    return std::dynamic_pointer_cast<ActorType>(Actor);
                }
            }
            return nullref;
        }

        // EVENTS

        /** Automatically called to create new actors. */
        virtual void Load();

        /** Automatically called after the creation of actors. */
        virtual void Start();

        /** Automatically called each tick. */
        virtual void Tick(const double &DeltaTime);

        /** Automatically called after each actor and component has already called Tick(). */
        virtual void LateTick(const double &DeltaTime);

        /** Automatically called after this world is unloaded. */
        virtual void Unload();

        // CASTING

        /** Returns whether this world is the given world type. */
        template<typename WorldType>
        [[nodiscard]] bool Is() const {
            return typeid(*this).hash_code() == typeid(WorldType).hash_code();
        }

        /**
         * Casts this world to the given world type.<br/>
         * Returns nullptr if the cast failed.
         */
        template<typename NewWorldType>
        [[nodiscard]] NewWorldType *Cast() {
            return dynamic_cast<NewWorldType *>(this);
        }

        // ACTIVITY

        /** Returns whether this world is active. */
        [[nodiscard]] bool IsActive() const;

        /**
         * Sets whether this world is active.<br/>
         * This can be overridden to control custom functionality.
         */
        virtual void SetActive(const bool &Active);
    };
}
