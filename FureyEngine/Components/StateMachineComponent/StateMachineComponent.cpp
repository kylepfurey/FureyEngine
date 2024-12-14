// .cpp
// State Machine Component Class Script
// by Kyle Furey

#include "StateMachineComponent.h"

#include <utility>

namespace FureyEngine {
    // CONSTRUCTORS

    // Constructs a new state machine component.
    StateMachineComponent::StateMachineComponent(const std::string &CurrentState,
                                                 std::map<std::string, std::function<void(
                                                     StateMachineComponent *, double)> > States,
                                                 const bool &TickLate)
        : Component("State", true), CurrentState(CurrentState), StartingState(CurrentState),
          States(std::move(States)), TickLate(TickLate) {
    }

    // EVENTS

    // Automatically called before each component has already called Start().
    void StateMachineComponent::Attach() {
        // Calls the base class's function
        Component::Attach();

        // Bind set active to the actor's set active function
        GetActor()->OnSetActive.Bind(std::bind(StateMachineComponent::SetActive, this, std::placeholders::_1));
    }

    // Automatically called when this component is first created.
    void StateMachineComponent::Start() {
        // Calls the base class's function
        Component::Start();

        // Switch to the starting state
        StateMachineComponent::SwitchState(CurrentState);
    }

    // Automatically called each world tick.
    void StateMachineComponent::Tick(const double &DeltaTime) {
        // Calls the base class's function
        Component::Tick(DeltaTime);

        // Tick the current state
        if (!TickLate) {
            if (CurrentState != NULL_STATE) {
                if (States.count(CurrentState) && States[CurrentState] != nullptr) {
                    States[CurrentState](this, DeltaTime);
                } else {
                    StateMachineComponent::SwitchState(NULL_STATE);
                }
            }
        }
    }

    // Automatically called after each component has already called Tick().
    void StateMachineComponent::LateTick(const double &DeltaTime) {
        // Calls the base class's function
        Component::LateTick(DeltaTime);

        // Tick the current state
        if (TickLate) {
            if (CurrentState != NULL_STATE) {
                if (States.count(CurrentState) && States[CurrentState] != nullptr) {
                    States[CurrentState](this, DeltaTime);
                } else {
                    StateMachineComponent::SwitchState(NULL_STATE);
                }
            }
        }
    }

    // Automatically called after this component is removed.
    void StateMachineComponent::Remove() {
        // Calls the base class's function
        Component::Remove();

        // Unbind set active from the actor's set active function
        if (BeginTick) {
            GetActor()->OnSetActive.Unbind(std::bind(StateMachineComponent::SetActive, this, std::placeholders::_1));
        }

        // Switch to the NULL state
        StateMachineComponent::SwitchState(NULL_STATE);
    }

    // STATE MACHINE

    // Returns whether the state machine component's current state is null.
    bool StateMachineComponent::IsStateNull() const {
        return CurrentState == NULL_STATE;
    }

    // Returns the name of the current state.
    std::string StateMachineComponent::GetCurrentState() const {
        return CurrentState;
    }

    // Returns the current state's function.
    std::function<void(StateMachineComponent *, double)> StateMachineComponent::GetFunction() const {
        return IsStateNull() ? nullptr : States.at(CurrentState);
    }

    // Returns whether the state machine component has the given state by name.
    bool StateMachineComponent::HasState(const std::string &State) const {
        return State != NULL_STATE && States.count(State);
    }

    // Switches the current state to the given state by name and returns whether the current state is not null.
    // Setting this to NULL_STATE will remove the current state.
    bool StateMachineComponent::SwitchState(const std::string &NewState) {
        // Exit the current state
        const std::string PreviousState = CurrentState;
        if (!IsStateNull()) {
            OnStateExit(this, NewState);
        }

        // Enter the new state
        CurrentState = HasState(NewState) ? NewState : NULL_STATE;
        if (!IsStateNull()) {
            OnStateEnter(this, PreviousState);
        }

        return CurrentState != NULL_STATE;
    }
}
