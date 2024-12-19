// .cpp
// Scriptable World Class Script
// by Kyle Furey

#include "World.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Constructs a new world with a name.
    World::World(std::string Name, const bool &Active)
        : Active(Active), Name(std::move(Name)) {
        ++TotalWorlds;
    }

    // DESTRUCTOR

    // Properly destroys this world.
    World::~World() {
        --TotalWorlds;
    }

    // TIME

    // Returns the time point this world was created.
    std::chrono::high_resolution_clock::time_point World::LoadTime() const {
        return LoadTimePoint;
    }

    // Returns the current time point as of this world's tick.
    std::chrono::high_resolution_clock::time_point World::CurrentTime() const {
        return CurrentTimePoint;
    }

    // The time in seconds since this world was last ticked.
    double World::DeltaTime() const {
        return CurrentDeltaTime;
    }

    // Returns the elapsed time (in seconds) since this world was created.
    double World::ElapsedTime() const {
        return static_cast<std::chrono::duration<double>>(CurrentTimePoint - LoadTimePoint).count();
    }

    // Returns the total number of times Tick() has been called for this world.
    unsigned long long World::TickCount() const {
        return TotalTicks;
    }

    // Invokes the given function after the given number of seconds.
    // Returns the scheduled event.
    World::TimedEvent *World::InvokeAfterSeconds(double Delay,
                                                 const std::function<void()> &Function) {
        ScheduledEvents.push_back({Delay, Function});
        return &ScheduledEvents.back();
    }

    // Invokes the given function after the given number of seconds for the given number of loops.
    // If loops is equal to -1, it will loop forever.
    // Returns the scheduled event.
    World::TimedEvent *World::RepeatAfterSeconds(double Delay,
                                                 const int &Loops,
                                                 const std::function<void()> &Function) {
        if (Loops == 0) {
            return nullptr;
        }
        if (Delay <= 0) {
            Delay = 0.000001;
        }
        ScheduledEvents.push_back({
            Delay, [=] {
                Function();
                RepeatAfterSeconds(Delay, Loops - 1, Function);
            }
        });
        return &ScheduledEvents.back();
    }

    // Invokes the given function after the given number of seconds for the given number of loops.
    // This function will loop forever unless cancelled.
    // Returns the scheduled event.
    World::TimedEvent *World::RepeatAfterSeconds(const double &Delay, const std::function<void()> &Function) {
        return RepeatAfterSeconds(Delay, -1, Function);
    }

    // Invokes the given function the following tick.
    // Returns the scheduled event.
    World::TimedEvent *World::InvokeNextTick(const std::function<void()> &Function) {
        return InvokeAfterSeconds(0.0000001, Function);
    }

    // Attempt to unschedule the given function from the scheduled events.
    // Returns if the cancellation was successful.
    bool World::CancelInvoke(const std::function<void()> &Function) {
        auto Iterator = ScheduledEvents.begin();
        for (const auto &[Delay, Event, Invoked]: ScheduledEvents) {
            if (Event.target<void()>() == Function.target<void()>()) {
                ScheduledEvents.erase(Iterator);
                return true;
            }
            ++Iterator;
        }
        return false;
    }

    // ACTORS

    // Updates the current state of this world and its actors.
    // Call this in each update loop.
    void World::Update() {
        // Check if this world is active
        if (!Active) {
            return;
        }

        if (BeginTick) {
            // TICK

            // Update delta time
            CurrentDeltaTime = static_cast<std::chrono::duration<double>>(
                                   std::chrono::high_resolution_clock::now() - CurrentTimePoint).count() * TimeScale;
            CurrentTimePoint = std::chrono::high_resolution_clock::now();

            // Ensure delta time is not 0
            if (CurrentDeltaTime <= 0) {
                CurrentDeltaTime = 0.0000001;
            }

            // Call and clear scheduled events
            auto Iterator = ScheduledEvents.begin();
            for (auto &[Delay, Event, Invoked]: ScheduledEvents) {
                Delay -= CurrentDeltaTime;
                ++Iterator;
            }

            Iterator = ScheduledEvents.begin();
            for (auto &[Delay, Event, Invoked]: ScheduledEvents) {
                if (Delay <= 0) {
                    Event();
                    Invoked = true;
                    Cleanup.Bind([&, Iterator] { ScheduledEvents.erase(Iterator); });
                }
                ++Iterator;
            }

            // Call world tick events
            Tick(CurrentDeltaTime);
            OnTick(this, CurrentDeltaTime);

            // Call actor tick events
            for (const auto &Actor: Actors) {
                if (!Actor->IsActive()) {
                    continue;
                }
                if (Actor->BeginTick) {
                    Actor->Tick(CurrentDeltaTime);
                    Actor->OnTick(Actor.get(), CurrentDeltaTime);
                } else {
                    Actor->Spawn();
                    Actor->OnSpawn(Actor.get());
                }

                // Call component tick events
                for (auto &Component: Actor->Components) {
                    if (!Component->IsActive()) {
                        continue;
                    }
                    if (Component->BeginTick) {
                        Component->Tick(CurrentDeltaTime);
                        Component->OnTick(Component.get(), CurrentDeltaTime);
                    } else {
                        Component->Attach();
                        Component->OnAttach(Component.get());
                    }
                }
            }

            // Call world late tick events
            LateTick(CurrentDeltaTime);
            OnLateTick(this, CurrentDeltaTime);

            // Call actor late tick events
            for (const auto &Actor: Actors) {
                if (!Actor->IsActive()) {
                    continue;
                }
                if (Actor->BeginTick) {
                    Actor->LateTick(CurrentDeltaTime);
                    Actor->OnLateTick(Actor.get(), CurrentDeltaTime);
                } else {
                    Actor->Start();
                    Actor->OnStart(Actor.get());
                    Actor->BeginTick = true;
                }

                // Call component late tick events
                for (auto &Component: Actor->Components) {
                    if (!Component->IsActive()) {
                        continue;
                    }
                    if (Component->BeginTick) {
                        Component->LateTick(CurrentDeltaTime);
                        Component->OnLateTick(Component.get(), CurrentDeltaTime);
                    } else {
                        Component->Start();
                        Component->OnStart(Component.get());
                        Component->BeginTick = true;
                    }
                }
            }
        } else {
            // START

            // Update the current time
            CurrentTimePoint = std::chrono::high_resolution_clock::now();

            // Call world load events
            Load();
            OnLoad(this);

            // Call actor tick events
            for (const auto &Actor: Actors) {
                if (!Actor->IsActive()) {
                    continue;
                }
                if (Actor->BeginTick) {
                    Actor->Tick(CurrentDeltaTime);
                    Actor->OnTick(Actor.get(), CurrentDeltaTime);
                } else {
                    Actor->Spawn();
                    Actor->OnSpawn(Actor.get());
                }

                // Call component tick events
                for (auto &Component: Actor->Components) {
                    if (!Component->IsActive()) {
                        continue;
                    }
                    if (Component->BeginTick) {
                        Component->Tick(CurrentDeltaTime);
                        Component->OnTick(Component.get(), CurrentDeltaTime);
                    } else {
                        Component->Attach();
                        Component->OnAttach(Component.get());
                    }
                }
            }

            // Call world start events
            Start();
            OnStart(this);

            // Call actor late tick events
            for (const auto &Actor: Actors) {
                if (!Actor->IsActive()) {
                    continue;
                }
                if (Actor->BeginTick) {
                    Actor->LateTick(CurrentDeltaTime);
                    Actor->OnLateTick(Actor.get(), CurrentDeltaTime);
                } else {
                    Actor->Start();
                    Actor->OnStart(Actor.get());
                    Actor->BeginTick = true;
                }

                // Call component late tick events
                for (auto &Component: Actor->Components) {
                    if (!Component->IsActive()) {
                        continue;
                    }
                    if (Component->BeginTick) {
                        Component->LateTick(CurrentDeltaTime);
                        Component->OnLateTick(Component.get(), CurrentDeltaTime);
                    } else {
                        Component->Start();
                        Component->OnStart(Component.get());
                        Component->BeginTick = true;
                    }
                }
            }

            BeginTick = true;
        }

        // Call cleanup events
        Cleanup();
        Cleanup.Clear();

        ++TotalTicks;
    }

    // Returns the first actor of the given type by name
    // or nullptr if it does not exist in this world.
    Reference<Actor> World::GetActor(const std::string &Name) const {
        for (auto &Actor: Actors) {
            if (Actor->Name == Name) {
                return Actor;
            }
        }
        return nullref;
    }

    // EVENTS

    // Automatically called to create new actors.
    void World::Load() {
        // std::cout << "World \"" << Name << "\" was loaded." << std::endl;
    }

    // Automatically called after the creation of actors.
    void World::Start() {
        // std::cout << "World \"" << Name << "\" started." << std::endl;
    }

    // Automatically called each tick.
    void World::Tick(const double &DeltaTime) {
        // std::cout << "World \"" << Name << "\" ticked." << std::endl;
    }

    // Automatically called after each actor and component has already called Tick().
    void World::LateTick(const double &DeltaTime) {
        // std::cout << "World \"" << Name << "\" ticked again." << std::endl;
    }

    // Automatically called after this world is unloaded.
    void World::Unload() {
        // std::cout << "World \"" << Name << "\" was unloaded." << std::endl;
    }

    // ACTIVITY

    // Returns whether this world is active.
    bool World::IsActive() const {
        return Active;
    }

    // Sets whether this world is active.
    // This can be overridden to control custom functionality.
    void World::SetActive(const bool &Active) {
        this->Active = Active;
        OnSetActive(Active);
    }

    // STATIC VARIABLE INITIALIZATION

    // The total number of worlds currently allocated.
    int World::TotalWorlds;
}
