// .cpp
// SDL Keyboard + Mouse Input Script
// by Kyle Furey

#include "Input.h"

namespace FureyEngine {
    // UPDATE

    // Polls the current SDL event and updates any events relying on input.
    // Call this in your main loop to update the user's input as fast as possible.
    // Takes the loop condition as a parameter for quitting logic.
    void Input::Update(bool &LoopCondition) {
        // Update delta time
        CurrentDeltaTime = static_cast<std::chrono::duration<double>>(
            std::chrono::high_resolution_clock::now() - CurrentTimePoint).count();
        CurrentTimePoint = std::chrono::high_resolution_clock::now();

        // Ensure delta time is not 0
        if (CurrentDeltaTime <= 0) {
            CurrentDeltaTime = 0.0000001;
        }

        // Store each event and input from this tick
        std::list<::SDL_Event> CurrentEvents;
        std::list<Sint32> CurrentInputs;

        // Updates the current event and key
        while (SDL_PollEvent(&MyEvent)) {
            // Execute specific event logic
            Sint32 Input = SDLK_UNKNOWN;
            switch (MyEvent.type) {
                // Unimplemented event
                default:
                    break;
                // Quit
                case SDL_QUIT:
                    // Call clean up events
                    Cleanup();

                    // Quit the program
                    LoopCondition = false;
                    StopText();
                    QuitEvent();
                    break;
#if KEYBOARD_INPUT
                // Key pressed
                case SDL_KEYDOWN:
                    Input = MyEvent.key.keysym.sym;
                    KeyStates[Input] = ButtonState::DOWN;
                    KeyboardEvents[Input](KeyStates[Input]);
                    break;
                // Key released
                case SDL_KEYUP:
                    Input = MyEvent.key.keysym.sym;
                    KeyStates[Input] = ButtonState::RELEASED;
                    KeyboardEvents[Input](KeyStates[Input]);
                    break;
                // Text input
                case SDL_TEXTINPUT:
                    CurrentText += MyEvent.text.text;
                    TextEvents(MyEvent.text.text[0], CurrentText);
                    break;
#endif
#if MOUSE_INPUT
                // Mouse button pressed
                case SDL_MOUSEBUTTONDOWN:
                    Input = MyEvent.button.button;
                    MouseButtonStates[Input] = ButtonState::DOWN;
                    MouseButtonEvents[Input](MousePosition, MouseButtonStates[Input]);
                    break;
                // Mouse button released
                case SDL_MOUSEBUTTONUP:
                    Input = MyEvent.button.button;
                    MouseButtonStates[Input] = ButtonState::RELEASED;
                    MouseButtonEvents[Input](MousePosition, MouseButtonStates[Input]);
                    break;
                // Mouse wheel moved
                case SDL_MOUSEWHEEL:
                    MouseWheelEvent(MyEvent.wheel.x, MyEvent.wheel.y);
                    break;
                // Mouse moved
                case SDL_MOUSEMOTION:
                    MousePosition = {MyEvent.motion.x, MyEvent.motion.y};
                    MouseMovementEvent(MousePosition, {MyEvent.motion.xrel, MyEvent.motion.yrel});
                    break;
#endif
            }

            // Store the current event and input
            CurrentEvents.push_back(MyEvent);
            CurrentInputs.push_back(Input);
        }

#if KEYBOARD_INPUT
        // Fire keyboard events
        for (auto &[Key, Value]: KeyStates) {
            if (std::find(CurrentInputs.begin(), CurrentInputs.end(), Key) != CurrentInputs.end()) {
                continue;
            }
            switch (Value) {
                case ButtonState::UP:
#if FIRE_WHILE_UP
                    KeyboardEvents[Key](Value);
#endif
                    continue;
                case ButtonState::DOWN:
                    Value = ButtonState::HELD;
#if FIRE_WHILE_HELD
                    KeyboardEvents[Key](Value);
#endif
                    continue;
                case ButtonState::HELD:
#if FIRE_WHILE_HELD
                    KeyboardEvents[Key](Value);
#endif
                    continue;
                case ButtonState::RELEASED:
                    Value = ButtonState::UP;
#if FIRE_WHILE_UP
                KeyboardEvents[Key](Value);
#endif
                    continue;
            }
        }
#endif

#if MOUSE_INPUT
        // Fire mouse button events
        for (auto &[Key, Value]: MouseButtonStates) {
            if (std::find(CurrentInputs.begin(), CurrentInputs.end(), Key) != CurrentInputs.end()) {
                continue;
            }
            switch (Value) {
                case ButtonState::UP:
#if FIRE_WHILE_UP
                    MouseButtonEvents[Key](MousePosition, Value);
#endif
                    continue;
                case ButtonState::DOWN:
                    Value = ButtonState::HELD;
#if FIRE_WHILE_HELD
                    MouseButtonEvents[Key](MousePosition, Value);
#endif
                    continue;
                case ButtonState::HELD:
#if FIRE_WHILE_HELD
                    MouseButtonEvents[Key](MousePosition, Value);
#endif
                    continue;
                case ButtonState::RELEASED:
                    Value = ButtonState::UP;
#if FIRE_WHILE_UP
                MouseButtonEvents[Key](MousePosition, Value);
#endif
                    continue;
            }
        }
#endif

        // Call the current events' corresponding delegate
        for (const ::SDL_Event Event: CurrentEvents) {
            Events[static_cast<SDL_EventType>(Event.type)](Event);
        }

        // Call update events
        UpdateEvents(CurrentDeltaTime);

        // Call cleanup events
        Cleanup();
        Cleanup.Clear();

        ++TotalUpdates;
    }

    // EVENTS

    // Returns the current SDL_Event associated with the last update.
    SDL_Event &Input::SDL_Event() {
        return MyEvent;
    }

    // QUIT

    // Exits the main loop the next update to quit the program.
    void Input::Quit() {
        const auto QuitEvent = std::make_shared<::SDL_Event>();
        QuitEvent->type = SDL_QUIT;
        SDL_PushEvent(QuitEvent.get());
    }

    // KEYBOARD

    // Returns the current state of the given key.
    ButtonState Input::GetKeyState(const SDL_Keycode &Key) {
        return KeyStates[Key];
    }

    // Returns whether the given key is currently up.
    bool Input::IsKeyUp(const SDL_Keycode &Key) {
        return KeyStates[Key] == ButtonState::UP;
    }

    // Returns whether the given key is currently down.
    bool Input::IsKeyDown(const SDL_Keycode &Key) {
        return KeyStates[Key] == ButtonState::DOWN;
    }

    // Returns whether the given key is currently held.
    bool Input::IsKeyHeld(const SDL_Keycode &Key) {
        return KeyStates[Key] == ButtonState::HELD;
    }

    // Returns whether the given key is currently released.
    bool Input::IsKeyReleased(const SDL_Keycode &Key) {
        return KeyStates[Key] == ButtonState::RELEASED;
    }

    // Starts receiving text input.
    void Input::StartText() {
        if (!TextInput) {
            TextInput = true;
            CurrentText = "";
            SDL_StartTextInput();
        }
    }

    // Stops receiving text input.
    // Returns the currently written text.
    std::string Input::StopText() {
        if (TextInput) {
            TextInput = false;
            std::string Input = CurrentText;
            CurrentText = "";
            SDL_StopTextInput();
            return Input;
        }

        return "";
    }

    // Clears and returns the current text input.
    std::string Input::ClearText() {
        if (TextInput) {
            std::string Input = CurrentText;
            CurrentText = "";
            return Input;
        }

        return "";
    }

    // Returns whether text input is currently enabled.
    bool Input::IsTextEnabled() {
        return TextInput;
    }

    // Returns the current text input.
    std::string Input::Text() {
        return CurrentText;
    }

    // Returns the most recent character pressed on the keyboard.
    char Input::Character() {
        if (CurrentText.empty()) {
            return '\0';
        }
        return CurrentText[CurrentText.size() - 1];
    }

    // MOUSE

    // Returns the current state of the given mouse button.
    ButtonState Input::GetMouseButtonState(const Uint8 &Button) {
        return MouseButtonStates[Button];
    }

    // Returns whether the given mouse button is currently up.
    bool Input::IsMouseButtonUp(const Uint8 &Button) {
        return MouseButtonStates[Button] == ButtonState::UP;
    }

    // Returns whether the given mouse button is currently down.
    bool Input::IsMouseButtonDown(const Uint8 &Button) {
        return MouseButtonStates[Button] == ButtonState::DOWN;
    }

    // Returns whether the given mouse button is currently held.
    bool Input::IsMouseButtonHeld(const Uint8 &Button) {
        return MouseButtonStates[Button] == ButtonState::HELD;
    }

    // Returns whether the given mouse button is currently released.
    bool Input::IsMouseButtonReleased(const Uint8 &Button) {
        return MouseButtonStates[Button] == ButtonState::RELEASED;
    }

    // MOUSE WHEEL

    // Returns the scrolled amount for the mouse scroll wheel on the horizontal axis.
    Sint32 Input::GetMouseWheelX() {
        if (MyEvent.type != SDL_MOUSEWHEEL) {
            return 0;
        }
        return MyEvent.wheel.x;
    }

    // Returns the scrolled amount for the mouse scroll wheel on the horizontal axis.
    Sint32 Input::GetMouseWheelY() {
        if (MyEvent.type != SDL_MOUSEWHEEL) {
            return 0;
        }
        return MyEvent.wheel.y;
    }

    // MOUSE POSITION

    // Returns the mouse's current position.
    glm::ivec2 Input::GetMousePosition() {
        return MousePosition;
    }

    // Returns the mouse's current change in X position.
    glm::ivec2 Input::GetMouseDelta() {
        if (MyEvent.type != SDL_MOUSEMOTION) {
            return {0, 0};
        }
        return {MyEvent.motion.xrel, MyEvent.motion.yrel};
    }

    // Returns the mouse's current change in position normalized.
    glm::vec2 Input::GetMouseDeltaNormalized() {
        if (MyEvent.type != SDL_MOUSEMOTION) {
            return {0, 0};
        }
        const glm::ivec2 Direction = {MyEvent.motion.xrel, MyEvent.motion.yrel};
        return Math::Normalize(Direction, DEADZONE);
    }

    // Shows the mouse cursor.
    void Input::ShowMouse() {
        SDL_ShowCursor(SDL_ENABLE);
    }

    // Hide the mouse cursor.
    void Input::HideMouse() {
        SDL_ShowCursor(SDL_DISABLE);
    }

    // Returns whether the mouse cursor is currently visible.
    bool Input::IsMouseVisible() {
        return SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE;
    }

    // DELTA TIME

    // Returns the time point the program started.
    std::chrono::high_resolution_clock::time_point Input::StartTime() {
        return StartingTimePoint;
    }

    // Returns the current time point as of this input update.
    std::chrono::high_resolution_clock::time_point Input::CurrentTime() {
        return CurrentTimePoint;
    }

    // The time in seconds since input was last updated.
    double Input::DeltaTime() {
        return CurrentDeltaTime;
    }

    // Returns the total number of times Update() has been called.
    unsigned long long Input::FrameCount() {
        return TotalUpdates;
    }

    // Returns the elapsed time  (in seconds) since the program started as of this update.
    double Input::ElapsedTime() {
        return static_cast<std::chrono::duration<double>>(CurrentTimePoint - StartingTimePoint).count();
    }

    // Returns the current frame rate as of this update.
    double Input::FrameRate() {
        const double ElapsedTime = static_cast<std::chrono::duration<double>>(CurrentTimePoint - StartingTimePoint).
                count();
        if (ElapsedTime == 0) {
            return 0;
        }
        return static_cast<double>(TotalUpdates) / ElapsedTime;
    }

    // STATIC VARIABLE INITIALIZATION

    // The current SDL event queued from this device this update.
    SDL_Event Input::MyEvent;

    // Each key and its current state.
    std::map<SDL_Keycode, ButtonState> Input::KeyStates;

    // Whether to read keyboard text input from the user.
    bool Input::TextInput;

    // The current text input from the keyboard.
    std::string Input::CurrentText;

    // Each mouse button and its current state.
    std::map<Uint8, ButtonState> Input::MouseButtonStates;

    // The mouse's current position.
    glm::ivec2 Input::MousePosition;

    // The time point the program was started.
    const std::chrono::high_resolution_clock::time_point Input::StartingTimePoint
            = std::chrono::high_resolution_clock::now();

    // The last updated time point.
    std::chrono::high_resolution_clock::time_point Input::CurrentTimePoint
            = StartingTimePoint;

    // The time in seconds since last updating input.
    double Input::CurrentDeltaTime = 0.0000001;

    // The total number of updates.
    unsigned long long Input::TotalUpdates;

    // Each event type to a delegate to invoke when that event is invoked.
    // Each delegate's functions take a const reference to an SDL_Event as its parameter.
    std::map<SDL_EventType, Delegate<const SDL_Event &> > Input::Events;

    // Functions to call on quitting.
    Event Input::QuitEvent;

    // Each keycode to a delegate to invoke when that key's state is changed.
    // Each delegate's functions take a ButtonState enum as its parameter.
    std::map<SDL_Keycode, Delegate<const ButtonState &> > Input::KeyboardEvents;

    /**
     * Events to call when text is updated with a keyboard while text input is enabled.<br/>
     * Each function takes a char for the input character and a string for the current text.
     */
    Delegate<char, std::string> Input::TextEvents;

    // Each mouse button to a delegate to invoke when that mouse button's state is changed.
    // Each delegate's functions take a glm::ivec2 and a ButtonState enum
    // as its parameters for the mouse's location.
    std::map<int, Delegate<const glm::ivec2 &, const ButtonState &> > Input::MouseButtonEvents;

    // Events to call when the mouse wheel is moved.
    // Each function takes two Uint32s as its parameters for delta X and Y.
    Delegate<const Sint32 &, const Sint32 &> Input::MouseWheelEvent;

    // Events to call when the mouse is moved.
    // Each function takes two glm::ivec2s as its parameters for current position and change in position.
    Delegate<const glm::ivec2 &, const glm::ivec2 &> Input::MouseMovementEvent;

    // Events to call each input update tick.
    // Each function takes a double as its parameter for the current delta time since the last update.
    Delegate<const double &> Input::UpdateEvents;

    // Events to call at the very end of an input update.
    // This event is cleared after each call.
    // Bind unbind calls to this to ensure they are called at the very end of the loop.
    Event Input::Cleanup;
}
