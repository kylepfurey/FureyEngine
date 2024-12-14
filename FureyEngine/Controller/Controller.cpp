// .cpp
// SDL Controller Input Script
// by Kyle Furey

#include "Controller.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Creates a new game controller.
    Controller::Controller() {
#if CONTROLLER_INPUT
        // Attempts to bind to a new controller
        if (!BindInput()) {
            // Awaits the next connection
            Input::Events[SDL_CONTROLLERDEVICEADDED].Bind
                    (std::bind(&Controller::OnConnect, this, std::placeholders::_1));
        }

        ++TotalControllers;
#endif
    }

    // Creates a new game controller and calls the given functions on connection.
    Controller::Controller(const std::initializer_list<std::function<void(const ::SDL_JoystickID &)> > &ConnectEvent) {
#if CONTROLLER_INPUT
        // Binds connection events
        for (int i = 0; i < ConnectEvent.size(); ++i) {
            this->ConnectEvent.Bind(*(ConnectEvent.begin() + i));
        }

        // Attempts to bind to a new controller
        if (!BindInput()) {
            // Awaits the next connection
            Input::Events[SDL_CONTROLLERDEVICEADDED].Bind
                    (std::bind(&Controller::OnConnect, this, std::placeholders::_1));
        }

        ++TotalControllers;
#endif
    }

    // DESTRUCTOR

    // Properly destroys this controller.
    Controller::~Controller() {
#if CONTROLLER_INPUT
        --TotalControllers;

        UnbindInput();
#endif
    }

    // GETTERS

    // Returns the unique SDL_JoystickID of this controller.
    // NOTE: This may change when the controller is reconnected but this will always represent this controller.
    SDL_JoystickID Controller::SDL_JoystickID() const {
        return JoystickID;
    }

    // Returns the pointer to this controller's SDL_Joystick.
    SDL_Joystick *Controller::SDL_Joystick() const {
        return Joystick;
    }

    // Returns the pointer to this controller's SDL_GameController.
    SDL_GameController *Controller::SDL_GameController() const {
        return MyController;
    }

    // Returns the total number of controllers currently active.
    int Controller::Total() {
        return TotalControllers;
    }

    // BINDINGS

    // Finds the next available controller if any is present.
    SDL_GameController *Controller::NewController() {
        // Store the number of joysticks
        const int TotalJoysticks = SDL_NumJoysticks();
        if (ActiveIDs.size() >= TotalJoysticks) {
            return nullptr;
        }

        // Loop through each joystick until we find a valid one
        for (int i = 0; i < TotalJoysticks; ++i) {
            // Did we find a valid controller?
            if (SDL_IsGameController(i)) {
                // Open the current controller
                if (::SDL_GameController *CurrentController = SDL_GameControllerOpen(i);
                    CurrentController != nullptr) {
                    // Is this controller's joystick ID already inside our active controllers list
                    if (!ActiveIDs.count(SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(CurrentController)))) {
                        return CurrentController;
                    }
                    // Deallocate the reference to the existing controller
                    SDL_GameControllerClose(CurrentController);
                } else {
                    throw std::runtime_error("ERROR: An SDL_GameController failed to initialize!\nSDL Error: " +
                                             std::string(SDL_GetError()));
                }
            }
        }

        return nullptr;
    }

    // Binds input to this controller.
    // Returns whether a controller was able to be bound to.
    bool Controller::BindInput() {
        // Attempt to locate a new controller
        MyController = NewController();
        if (MyController == nullptr) {
            return false;
        }

        // Stores this game controller's joystick and its joystick ID
        Joystick = SDL_GameControllerGetJoystick(MyController);
        JoystickID = SDL_JoystickInstanceID(Joystick);
        ActiveIDs.insert(JoystickID);

        // Call connection events
        ConnectEvent(JoystickID);

        // Bind all events
        Input::Events[SDL_CONTROLLERBUTTONDOWN].
                Bind(std::bind(&Controller::OnButtonDown, this, std::placeholders::_1));
        Input::Events[SDL_CONTROLLERBUTTONUP].
                Bind(std::bind(&Controller::OnButtonUp, this, std::placeholders::_1));
        Input::Events[SDL_CONTROLLERAXISMOTION].
                Bind(std::bind(&Controller::OnMoveAxis, this, std::placeholders::_1));
        Input::Events[SDL_CONTROLLERDEVICEREMOVED].
                Bind(std::bind(&Controller::OnDisconnect, this, std::placeholders::_1));
        Input::UpdateEvents.
                Bind(std::bind(&Controller::Update, this, std::placeholders::_1));

        return true;
    }

    // Unbinds input from this controller.
    void Controller::UnbindInput() {
        // Unbind all events
        if (MyController == nullptr) {
            Input::Cleanup.Bind([&] {
                Input::Events[SDL_CONTROLLERDEVICEADDED].Unbind
                        (std::bind(&Controller::OnDisconnect, this, std::placeholders::_1));
            });
        } else {
            Input::Cleanup.Bind([&] {
                Input::UpdateEvents.Unbind
                        (std::bind(&Controller::Update, this, std::placeholders::_1));
            });
            Input::Cleanup.Bind([&] {
                Input::Events[SDL_CONTROLLERDEVICEREMOVED].Unbind
                        (std::bind(&Controller::OnDisconnect, this, std::placeholders::_1));
            });
            Input::Cleanup.Bind([&] {
                Input::Events[SDL_CONTROLLERAXISMOTION].Unbind
                        (std::bind(&Controller::OnMoveAxis, this, std::placeholders::_1));
            });
            Input::Cleanup.Bind([&] {
                Input::Events[SDL_CONTROLLERBUTTONUP].Unbind
                        (std::bind(&Controller::OnButtonUp, this, std::placeholders::_1));
            });
            Input::Cleanup.Bind([&] {
                Input::Events[SDL_CONTROLLERBUTTONDOWN].Unbind
                        (std::bind(&Controller::OnButtonDown, this, std::placeholders::_1));
            });

            // Call disconnection events
            DisconnectEvent(JoystickID);

            // Deletes this controller
            ActiveIDs.erase(JoystickID);
            JoystickID = 0;
            Joystick = nullptr;
            SDL_GameControllerClose(MyController);
            MyController = nullptr;
        }
    }

    // Called when a new controller is connected.
    void Controller::OnConnect(const SDL_Event &Event) {
        // Check for a successful binding
        if (BindInput()) {
            // Stop waiting for another connection
            Input::Cleanup.Bind([&] {
                Input::Events[SDL_CONTROLLERDEVICEADDED].Unbind
                        (std::bind(&Controller::OnConnect, this, std::placeholders::_1));
            });
        }
    }

    // Called when a controller is disconnected.
    void Controller::OnDisconnect(const SDL_Event &Event) {
        if (Event.cdevice.which == JoystickID) {
            // Unbinds this controller's input cycle from the input update loop
            UnbindInput();

            // Awaits the next connection
            Input::Events[SDL_CONTROLLERDEVICEADDED].Bind
                    (std::bind(&Controller::OnConnect, this, std::placeholders::_1));
        }
    }

    // BUTTON

    // Called when a controller button is pressed.
    void Controller::OnButtonDown(const SDL_Event &Event) {
        if (Event.cbutton.which != JoystickID) {
            return;
        }
        Button = Event.cbutton.button;
        ButtonStates[Button] = ButtonState::DOWN;
        ButtonEvents[static_cast<SDL_GameControllerButton>(Button)]
                (ButtonStates[static_cast<SDL_GameControllerButton>(Button)]);
    }

    // Called when a controller button is released.
    void Controller::OnButtonUp(const SDL_Event &Event) {
        if (Event.cbutton.which != JoystickID) {
            return;
        }
        Button = Event.cbutton.button;
        ButtonStates[Button] = ButtonState::RELEASED;
        ButtonEvents[static_cast<SDL_GameControllerButton>(Button)]
                (ButtonStates[static_cast<SDL_GameControllerButton>(Button)]);
    }

    // Returns the current state of the given button.
    ButtonState Controller::GetButtonState(const SDL_GameControllerButton &Button) const {
        return ButtonStates.at(Button);
    }

    // Returns whether the given button is currently up.
    bool Controller::IsButtonUp(const SDL_GameControllerButton &Button) const {
        return ButtonStates.at(Button) == ButtonState::UP;
    }

    // Returns whether the given button is currently down.
    bool Controller::IsButtonDown(const SDL_GameControllerButton &Button) const {
        return ButtonStates.at(Button) == ButtonState::DOWN;
    }

    // Returns whether the given button is currently held.
    bool Controller::IsButtonHeld(const SDL_GameControllerButton &Button) const {
        return ButtonStates.at(Button) == ButtonState::HELD;
    }

    // Returns whether the given button is currently released.
    bool Controller::IsButtonReleased(const SDL_GameControllerButton &Button) const {
        return ButtonStates.at(Button) == ButtonState::RELEASED;
    }

    // AXIS

    // Called when a controller axis value is changed.
    void Controller::OnMoveAxis(const SDL_Event &Event) {
        if (Event.caxis.which != JoystickID) {
            return;
        }
        switch (Event.caxis.axis) {
            default:
                break;
            case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
                LeftTriggerAxis = static_cast<float>(Event.caxis.value) / SDL_JOYSTICK_AXIS_MAX;
                break;
            case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
                RightTriggerAxis = static_cast<float>(Event.caxis.value) / SDL_JOYSTICK_AXIS_MAX;
                break;
            case SDL_CONTROLLER_AXIS_LEFTX:
                LeftStick.x = static_cast<float>(Event.caxis.value) / SDL_JOYSTICK_AXIS_MAX;
                LeftStickEvent(LeftStick);
                break;
            case SDL_CONTROLLER_AXIS_LEFTY:
                LeftStick.y = static_cast<float>(Event.caxis.value) / -SDL_JOYSTICK_AXIS_MAX;
                LeftStickEvent(LeftStick);
                break;
            case SDL_CONTROLLER_AXIS_RIGHTX:
                RightStick.x = static_cast<float>(Event.caxis.value) / SDL_JOYSTICK_AXIS_MAX;
                RightStickEvent(RightStick);
                break;
            case SDL_CONTROLLER_AXIS_RIGHTY:
                RightStick.y = static_cast<float>(Event.caxis.value) / -SDL_JOYSTICK_AXIS_MAX;
                RightStickEvent(RightStick);
                break;
        }

        // Normalizes the input axes for better utility
        NormalizeAxes();
    }

    // Clamps all axis values and implements a deadzone for stick axes.
    void Controller::NormalizeAxes() {
        LeftTriggerAxis = SDL_clamp(LeftTriggerAxis, 0, 1);
        RightTriggerAxis = SDL_clamp(LeftTriggerAxis, 0, 1);
        LeftStick.x = SDL_clamp(LeftStick.x, -1, 1);
        LeftStick.y = SDL_clamp(LeftStick.y, -1, 1);
        RightStick.x = SDL_clamp(RightStick.x, -1, 1);
        RightStick.y = SDL_clamp(RightStick.y, -1, 1);
        if (SDL_fabs(LeftStick.x) <= DEADZONE) {
            LeftStick.x = 0;
        }
        if (SDL_fabs(LeftStick.y) <= DEADZONE) {
            LeftStick.y = 0;
        }
        if (SDL_fabs(RightStick.x) <= DEADZONE) {
            RightStick.x = 0;
        }
        if (SDL_fabs(RightStick.y) <= DEADZONE) {
            RightStick.y = 0;
        }
    }

    // Returns the current value of the given axis.
    float Controller::GetAxis(const SDL_GameControllerAxis &Axis) const {
        switch (Axis) {
            default:
                return 0;
            case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
                return LeftTriggerAxis;
            case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
                return RightTriggerAxis;
            case SDL_CONTROLLER_AXIS_LEFTX:
                return LeftStick.x;
            case SDL_CONTROLLER_AXIS_LEFTY:
                return LeftStick.y;
            case SDL_CONTROLLER_AXIS_RIGHTX:
                return RightStick.x;
            case SDL_CONTROLLER_AXIS_RIGHTY:
                return RightStick.y;
        }
    }

    // TRIGGER

    // Returns the current value of the left trigger axis.
    float Controller::GetLeftTrigger() const {
        return LeftTriggerAxis;
    }

    // Returns the current state of the left trigger.
    ButtonState Controller::GetLeftTriggerState() const {
        return LeftTriggerState;
    }

    // Returns the current value of the right trigger axis.
    float Controller::GetRightTrigger() const {
        return RightTriggerAxis;
    }

    // Returns the current state of the right trigger.
    ButtonState Controller::GetRightTriggerState() const {
        return RightTriggerState;
    }

    // STICK

    // Returns the current direction of the left stick.
    glm::vec2 Controller::GetLeftStick() const {
        return LeftStick;
    }

    // Returns the current normalized direction of the left stick.
    glm::vec2 Controller::GetLeftStickNormalized() const {
        return Math::Normalize(LeftStick, DEADZONE);
    }

    // Returns the current direction of the right stick.
    glm::vec2 Controller::GetRightStick() const {
        return RightStick;
    }

    // Returns the current normalized direction of the right stick.
    glm::vec2 Controller::GetRightStickNormalized() const {
        return Math::Normalize(RightStick, DEADZONE);
    }

    // GAME CONTROLLER

    // Fires controller button events.
    void Controller::Update(const double &DeltaTime) {
#if CONTROLLER_INPUT
        // Fire controller button events
        for (auto &[Key, Value]: ButtonStates) {
            if (Key == Button) {
                continue;
            }
            switch (Value) {
                case ButtonState::UP:
#if FIRE_WHILE_UP
                    ButtonEvents[static_cast<SDL_GameControllerButton>(Key)](Value);
#endif
                    continue;
                case ButtonState::DOWN:
                    Value = ButtonState::HELD;
#if FIRE_WHILE_HELD
                    ButtonEvents[static_cast<SDL_GameControllerButton>(Key)](Value);
#endif
                    continue;
                case ButtonState::HELD:
#if FIRE_WHILE_HELD
                    ButtonEvents[static_cast<SDL_GameControllerButton>(Key)](Value);
#endif
                    continue;
                case ButtonState::RELEASED:
                    Value = ButtonState::UP;
#if FIRE_WHILE_UP
                ButtonEvents[static_cast<SDL_GameControllerButton>(Key)](Value);
#endif
                    continue;
            }
        }

        // Fire left trigger events
        switch (LeftTriggerState) {
            case ButtonState::UP:
                if (LeftTriggerAxis > DEADZONE) {
                    LeftTriggerState = ButtonState::DOWN;
                    LeftTriggerEvent(LeftTriggerAxis, LeftTriggerState);
                } else {
#if FIRE_WHILE_UP
                    LeftTriggerEvent(LeftTriggerAxis, LeftTriggerState);
#endif
                }
                break;
            case ButtonState::DOWN:
                if (LeftTriggerAxis > DEADZONE) {
                    LeftTriggerState = ButtonState::HELD;
#if FIRE_WHILE_HELD
                    LeftTriggerEvent(LeftTriggerAxis, LeftTriggerState);
#endif
                } else {
                    LeftTriggerState = ButtonState::RELEASED;
                    LeftTriggerEvent(LeftTriggerAxis, LeftTriggerState);
                }
                break;
            case ButtonState::HELD:
                if (LeftTriggerAxis > DEADZONE) {
#if FIRE_WHILE_HELD
                    LeftTriggerEvent(LeftTriggerAxis, LeftTriggerState);
#endif
                } else {
                    LeftTriggerState = ButtonState::RELEASED;
                    LeftTriggerEvent(LeftTriggerAxis, LeftTriggerState);
                }
                break;
            case ButtonState::RELEASED:
                if (LeftTriggerAxis > DEADZONE) {
                    LeftTriggerState = ButtonState::DOWN;
                    LeftTriggerEvent(LeftTriggerAxis, LeftTriggerState);
                } else {
                    LeftTriggerState = ButtonState::UP;
#if FIRE_WHILE_UP
                    LeftTriggerEvent(LeftTriggerAxis, LeftTriggerState);
#endif
                }
                break;
        }

        // Fire right trigger events
        switch (RightTriggerState) {
            case ButtonState::UP:
                if (RightTriggerAxis > DEADZONE) {
                    RightTriggerState = ButtonState::DOWN;
                    RightTriggerEvent(RightTriggerAxis, RightTriggerState);
                } else {
#if FIRE_WHILE_UP
                    RightTriggerEvent(RightTriggerAxis, RightTriggerState);
#endif
                }
                break;
            case ButtonState::DOWN:
                if (RightTriggerAxis > DEADZONE) {
                    RightTriggerState = ButtonState::HELD;
#if FIRE_WHILE_HELD
                    RightTriggerEvent(RightTriggerAxis, RightTriggerState);
#endif
                } else {
                    RightTriggerState = ButtonState::RELEASED;
                    RightTriggerEvent(RightTriggerAxis, RightTriggerState);
                }
                break;
            case ButtonState::HELD:
                if (RightTriggerAxis > DEADZONE) {
#if FIRE_WHILE_HELD
                    RightTriggerEvent(RightTriggerAxis, RightTriggerState);
#endif
                } else {
                    RightTriggerState = ButtonState::RELEASED;
                    RightTriggerEvent(RightTriggerAxis, RightTriggerState);
                }
                break;
            case ButtonState::RELEASED:
                if (RightTriggerAxis > DEADZONE) {
                    RightTriggerState = ButtonState::DOWN;
                    RightTriggerEvent(RightTriggerAxis, RightTriggerState);
                } else {
                    RightTriggerState = ButtonState::UP;
#if FIRE_WHILE_UP
                    RightTriggerEvent(RightTriggerAxis, RightTriggerState);
#endif
                }
                break;
        }

        // Reset the updated button and axis
        Button = SDL_CONTROLLER_BUTTON_INVALID;
#endif
    }

    // Returns whether this controller is connected.
    bool Controller::IsConnected() const {
        return MyController != nullptr;
    }

    // Returns whether this controller is connected.
    Controller::operator bool() const {
        return IsConnected();
    }

    // STATIC VARIABLE INITIALIZATION

    // The total number of active controllers.
    int Controller::TotalControllers;

    // Each connected joystick ID.
    std::set<SDL_JoystickID> Controller::ActiveIDs;
}
