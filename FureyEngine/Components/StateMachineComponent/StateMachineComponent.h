// .h
// State Machine Component Class Script
// by Kyle Furey

#pragma once
#include "../../Engine/Engine.h"

#define NULL_STATE "NULL"

namespace FureyEngine {
    /** A component used to bind logic to states and then switch between different states for branching logic. */
    class StateMachineComponent : public Component {
    protected:
        // STATE MACHINE

        /** The name of the current state. */
        std::string CurrentState = "NULL";

    public:
        // STATE MACHINE

        /** The name of the starting state. */
        const std::string StartingState;

        /**
         * A map of each state's name to its tick function in the state machine component.<br/>
         * Each state's function returns void and has two parameters:<br/>
         * The first parameter is a pointer to this state machine component.<br/>
         * The second parameter is a double for delta time.
         */
        std::map<std::string, std::function<void(StateMachineComponent *, double)> > States;

        /**
         * An event to call when a new state is first entered.<br/>
         * The first parameter is a pointer to this state machine component.<br/>
         * The second parameter is a string with the name of the previous state.
         */
        Delegate<StateMachineComponent *, std::string> OnStateEnter;

        /**
         * An event to call when the current state is switched.<br/>
         * The first parameter is a pointer to this state machine component.<br/>
         * The second parameter is a string with the name of the new state.
         */
        Delegate<StateMachineComponent *, std::string> OnStateExit;

        /** Whether to use LateTick() instead of Tick(). */
        bool TickLate = false;

        // CONSTRUCTORS

        /** Constructs a new state machine component. */
        explicit StateMachineComponent(const std::string &CurrentState = NULL_STATE,
                                       std::map<std::string, std::function<void(StateMachineComponent *, double)> >
                                       States = {},
                                       const bool &TickLate = false);

        // EVENTS

        /** Automatically called before each component has already called Start(). */
        void Attach() override;

        /** Automatically called when this component is first created. */
        void Start() override;

        /** Automatically called each world tick. */
        void Tick(const double &DeltaTime) override;

        /** Automatically called after each component has already called Tick(). */
        void LateTick(const double &DeltaTime) override;

        /** Automatically called after this component is removed. */
        void Remove() override;

        // STATE MACHINE

        /** Returns whether the state machine component's current state is null. */
        [[nodiscard]] virtual bool IsStateNull() const;

        /** Returns the name of the current state. */
        [[nodiscard]] virtual std::string GetCurrentState() const;

        /** Returns the current state's function. */
        [[nodiscard]] virtual std::function<void(StateMachineComponent *, double)> GetFunction() const;

        /** Returns whether the state machine component has the given state by name. */
        [[nodiscard]] virtual bool HasState(const std::string &State) const;

        /**
         * Switches the current state to the given state by name and returns whether the current state is not null.<br/>
         * Setting this to NULL_STATE will remove the current state.
         */
        virtual bool SwitchState(const std::string &NewState);
    };
}
