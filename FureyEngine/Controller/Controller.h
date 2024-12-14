// .h
// SDL Controller Input Script
// by Kyle Furey

#pragma once
#include "../Input/Input.h"

namespace FureyEngine {
    /**
     * Represents an automatically binding SDL game controller that can handle and send user input events.<br/>
     * This controller is automatically updated when Input::Update() is called.
     */
    class Controller final {
        // JOYSTICK

        /** The ID of this controller. */
        SDL_JoystickID JoystickID = 0;

        /** This controller's SDL joystick. */
        SDL_Joystick *Joystick = nullptr;

        // GAME CONTROLLER

        /** This controller's SDL game controller. */
        SDL_GameController *MyController = nullptr;

        // BUTTON

        /** The last updated button. */
        Uint8 Button = SDL_CONTROLLER_BUTTON_INVALID;

        /** Each game controller button and its current state. */
        std::map<Uint8, ButtonState> ButtonStates;

        // TRIGGER

        /** The current axis of the left trigger. */
        float LeftTriggerAxis = 0;

        /** The current state of the left trigger. */
        ButtonState LeftTriggerState = ButtonState::UP;

        /** The current axis of the right trigger. */
        float RightTriggerAxis = 0;

        /** The current state of the right trigger. */
        ButtonState RightTriggerState = ButtonState::UP;

        // STICK

        /** The current value of the left stick. */
        glm::vec2 LeftStick = {0, 0};

        /** The current value of the right stick. */
        glm::vec2 RightStick = {0, 0};

        // TOTAL CONTROLLERS

        /** The total number of active controllers. */
        static int TotalControllers;

        /** Each connected joystick ID. */
        static std::set<SDL_JoystickID> ActiveIDs;

        // BINDINGS

        /** Finds the next available controller if any is present. */
        static SDL_GameController *NewController();

        /**
         * Binds input to this controller.<br/>
         * Returns whether a controller was able to be bound to.
         */
        bool BindInput();

        /** Unbinds input from this controller. */
        void UnbindInput();

        /** Called when a new controller is connected. */
        void OnConnect(const SDL_Event &Event);

        /** Called when a controller is disconnected. */
        void OnDisconnect(const SDL_Event &Event);

        // BUTTON

        /** Called when a controller button is pressed. */
        void OnButtonDown(const SDL_Event &Event);

        /** Called when a controller button is released. */
        void OnButtonUp(const SDL_Event &Event);

        // AXIS

        /** Called when a controller axis value is changed. */
        void OnMoveAxis(const SDL_Event &Event);

        /** Clamps all axis values and implements a deadzone for stick axes. */
        void NormalizeAxes();

        // GAME CONTROLLER

        /** Fires controller button events. */
        void Update(const double &DeltaTime);

    public:
        // BINDINGS

        /**
         * Events to call when the controller is connected.<br/>
         * Each function takes an SDL_JoystickID as its parameter for this controller's joystick ID.
         */
        Delegate<const SDL_JoystickID &> ConnectEvent;

        /**
         * Events to call when the controller disconnects.<br/>
         * Each function takes an SDL_JoystickID as its parameter for this controller's joystick ID.
         */
        Delegate<const SDL_JoystickID &> DisconnectEvent;

        // BUTTON

        /**
         * Each game controller button to a delegate to invoke when that button's state is changed.<br/>
         * Each delegate's functions take a ButtonState enum as its parameter.
         */
        std::map<SDL_GameControllerButton, Delegate<const ButtonState &> > ButtonEvents;

        // TRIGGER

        /**
         * Events to call when the left trigger is pulled.<br/>
         * Each function takes a Sint16 and a ButtonState for the current axis and state of the left trigger.
         */
        Delegate<const float &, const ButtonState &> LeftTriggerEvent;

        /**
         * Events to call when the right trigger is pulled.<br/>
         * Each function takes a Sint16 and a ButtonState for the current axis and state of the right trigger.
         */
        Delegate<const float &, const ButtonState &> RightTriggerEvent;

        // STICK

        /**
         * Events to call when the left stick is moved.<br/>
         * Each function takes a glm::vec2 as its parameter for the current direction of the left stick.
         */
        Delegate<const glm::vec2 &> LeftStickEvent;

        /**
         * Events to call when the right stick is moved.<br/>
         * Each function takes a glm::vec2 as its parameter for the current direction of the right stick.
         */
        Delegate<const glm::vec2 &> RightStickEvent;

        // CONSTRUCTORS

        /** Creates a new game controller. */
        Controller();

        /** Creates a new game controller and calls the given functions on connection. */
        Controller(const std::initializer_list<std::function<void(const SDL_JoystickID &)> > &ConnectEvent);

        /** Delete copy constructor. */
        Controller(const Controller &Copied) = delete;

        /** Delete move constructor. */
        Controller(Controller &&Moved) = delete;

        // ASSIGNMENT OPERATORS

        /** Delete copy constructor. */
        Controller &operator=(const Controller &Copied) = delete;

        /** Delete move constructor. */
        Controller &operator=(Controller &&Moved) = delete;

        // DESTRUCTOR

        /** Properly destroys this controller. */
        ~Controller();

        // GETTERS

        /**
         * Returns the unique SDL_JoystickID of this controller.<br/>
         * NOTE: This may change when the controller is reconnected but this will always represent this controller.
         */
        [[nodiscard]] SDL_JoystickID SDL_JoystickID() const;

        /** Returns the pointer to this controller's SDL_GameController. */
        [[nodiscard]] SDL_GameController *SDL_GameController() const;

        /** Returns the pointer to this controller's SDL_Joystick. */
        [[nodiscard]] SDL_Joystick *SDL_Joystick() const;

        /** Returns the total number of controllers currently active. */
        [[nodiscard]] static int Total();

        // BUTTON

        /** Returns the current state of the given button. */
        [[nodiscard]] ButtonState GetButtonState(const SDL_GameControllerButton &Button) const;

        /** Returns whether the given button is currently up. */
        [[nodiscard]] bool IsButtonUp(const SDL_GameControllerButton &Button) const;

        /** Returns whether the given button is currently down. */
        [[nodiscard]] bool IsButtonDown(const SDL_GameControllerButton &Button) const;

        /** Returns whether the given button is currently held. */
        [[nodiscard]] bool IsButtonHeld(const SDL_GameControllerButton &Button) const;

        /** Returns whether the given button is currently released. */
        [[nodiscard]] bool IsButtonReleased(const SDL_GameControllerButton &Button) const;

        // AXIS

        /** Returns the current value of the given axis. */
        [[nodiscard]] float GetAxis(const SDL_GameControllerAxis &Axis) const;

        // TRIGGER

        /** Returns the current value of the left trigger axis. */
        [[nodiscard]] float GetLeftTrigger() const;

        /** Returns the current state of the left trigger. */
        [[nodiscard]] ButtonState GetLeftTriggerState() const;

        /** Returns the current value of the right trigger axis. */
        [[nodiscard]] float GetRightTrigger() const;

        /** Returns the current state of the right trigger. */
        [[nodiscard]] ButtonState GetRightTriggerState() const;

        // STICK

        /** Returns the current direction of the left stick. */
        [[nodiscard]] glm::vec2 GetLeftStick() const;

        /** Returns the current normalized direction of the left stick. */
        [[nodiscard]] glm::vec2 GetLeftStickNormalized() const;

        /** Returns the current direction of the right stick. */
        [[nodiscard]] glm::vec2 GetRightStick() const;

        /** Returns the current normalized direction of the right stick. */
        [[nodiscard]] glm::vec2 GetRightStickNormalized() const;

        // GAME CONTROLLER

        /** Returns whether this controller is connected. */
        [[nodiscard]] bool IsConnected() const;

        /** Returns whether this controller is connected. */
        explicit operator bool() const;
    };
}
