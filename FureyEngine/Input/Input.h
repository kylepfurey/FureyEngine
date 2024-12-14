// .h
// SDL Keyboard + Mouse Input Script
// by Kyle Furey

#pragma once
#include "../Math/Math.h"
#include "../Delegate/Delegate.h"
#include "../Event/Event.h"

#define KEYBOARD_INPUT 1
#define MOUSE_INPUT 1
#define CONTROLLER_INPUT 1
#define FIRE_WHILE_UP 0
#define FIRE_WHILE_HELD 1
#define LEFT_MOUSE_BUTTON 1
#define RIGHT_MOUSE_BUTTON 3
#define DEADZONE 0.1

namespace FureyEngine {
    /** The current state of a button or key. */
    enum class ButtonState {
        /** When a button or key is untouched. */
        UP = 0,

        /** When a button or key is first pressed. */
        DOWN = 1,

        /** When a button or key is held down after being pressed. */
        HELD = 2,

        /** When a button or key is no longer held after being held. */
        RELEASED = 3
    };

    /** A static class used to handle SDL events and user input. */
    class Input final {
        // ABSTRACT CLASS

        /** Prevents instantiation of this class. */
        virtual void Abstract() = 0;

        // EVENTS

        /** The current SDL event queued from this device this update. */
        static SDL_Event MyEvent;

        // KEYBOARD

        /** Each key and its current state. */
        static std::map<SDL_Keycode, ButtonState> KeyStates;

        /** Whether to read keyboard input from the user. */
        static bool TextInput;

        /** The current string input from the keyboard. */
        static std::string CurrentText;

        // MOUSE BUTTON

        /** Each mouse button and its current state. */
        static std::map<Uint8, ButtonState> MouseButtonStates;

        // MOUSE POSITION

        /** The mouse's current position. */
        static glm::ivec2 MousePosition;

        // DELTA TIME

        /** The time point the program was started. */
        static const std::chrono::high_resolution_clock::time_point StartingTimePoint;

        /** The last updated time point. */
        static std::chrono::high_resolution_clock::time_point CurrentTimePoint;

        /** The time in seconds since last updating input. */
        static double CurrentDeltaTime;

        /** The total number of updates. */
        static unsigned long long TotalUpdates;

    public:
        // EVENTS

        /**
         * Each event type to a delegate to invoke when that event is invoked.<br/>
         * Each delegate's functions take a const reference to an SDL_Event as its parameter.
         */
        static std::map<SDL_EventType, Delegate<const SDL_Event &> > Events;

        // QUIT

        /** Functions to call on quitting. */
        static Event QuitEvent;

        // KEYBOARD

        /**
         * Each keycode to a delegate to invoke when that key's state is changed.<br/>
         * Each delegate's functions take a ButtonState enum as its parameter.
         */
        static std::map<SDL_Keycode, Delegate<const ButtonState &> > KeyboardEvents;

        /**
         * Events to call when text is updated with a keyboard while text input is enabled.<br/>
         * Each function takes a char for the input character and a string for the current text.
         */
        static Delegate<char, std::string> TextEvents;

        // MOUSE BUTTON

        /**
         * Each mouse button to a delegate to invoke when that mouse button's state is changed.<br/>
         * Each delegate's functions take a glm::ivec2 and a ButtonState enum<br/>
         * as its parameters for the mouse's location.
         */
        static std::map<int, Delegate<const glm::ivec2 &, const ButtonState &> > MouseButtonEvents;

        // MOUSE WHEEL

        /**
         * Events to call when the mouse wheel is moved.<br/>
         * Each function takes two Sint32s as its parameters for delta X and Y.
         */
        static Delegate<const Sint32 &, const Sint32 &> MouseWheelEvent;

        // MOUSE POSITION

        /**
         * Events to call when the mouse is moved.<br/>
         * Each function takes two glm::ivec2s as its parameters for current position and change in position.
         */
        static Delegate<const glm::ivec2 &, const glm::ivec2 &> MouseMovementEvent;

        // DELTA TIME

        /**
         * Events to call each input update tick.<br/>
         * Each function takes a double as its parameter for the current delta time since the last update.
         */
        static Delegate<const double &> UpdateEvents;

        // CLEANUP

        /**
         * Events to call at the very end of an input update.<br/>
         * This event is cleared after each call.<br/>
         * Bind unbind calls to this to ensure they are called at the very end of the loop.
         */
        static Event Cleanup;

        // UPDATE

        /**
         * Polls the current SDL event and updates any events relying on input.<br/>
         * Call this in your main loop to update the user's input as fast as possible.<br/>
         * Takes the loop condition as a parameter for quitting logic.
         */
        static void Update(bool &LoopCondition);

        // EVENTS

        /** Returns the current SDL_Event associated with the last update. */
        [[nodiscard]] static SDL_Event &SDL_Event();

        // QUIT

        /** Exits the main loop the next update to quit the program. */
        static void Quit();

        // KEYBOARD

        /** Returns the current state of the given key. */
        [[nodiscard]] static ButtonState GetKeyState(const SDL_Keycode &Key);

        /** Returns whether the given key is currently up. */
        [[nodiscard]] static bool IsKeyUp(const SDL_Keycode &Key);

        /** Returns whether the given key is currently down. */
        [[nodiscard]] static bool IsKeyDown(const SDL_Keycode &Key);

        /** Returns whether the given key is currently held. */
        [[nodiscard]] static bool IsKeyHeld(const SDL_Keycode &Key);

        /** Returns whether the given key is currently released. */
        [[nodiscard]] static bool IsKeyReleased(const SDL_Keycode &Key);

        /** Starts receiving text input. */
        static void StartText();

        /**
         * Stops receiving text input.<br/>
         * Returns the currently written text.
         */
        static std::string StopText();

        /** Clears and returns the current text input. */
        static std::string ClearText();

        /** Returns whether text input is currently enabled. */
        [[nodiscard]] static bool IsTextEnabled();

        /** Returns the current text input. */
        [[nodiscard]] static std::string Text();

        /** Returns the most recent character pressed on the keyboard. */
        [[nodiscard]] static char Character();

        // MOUSE BUTTON

        /** Returns the current state of the given mouse button. */
        [[nodiscard]] static ButtonState GetMouseButtonState(const Uint8 &Button = LEFT_MOUSE_BUTTON);

        /** Returns whether the given mouse button is currently up. */
        [[nodiscard]] static bool IsMouseButtonUp(const Uint8 &Button = LEFT_MOUSE_BUTTON);

        /** Returns whether the given mouse button is currently down. */
        [[nodiscard]] static bool IsMouseButtonDown(const Uint8 &Button = LEFT_MOUSE_BUTTON);

        /** Returns whether the given mouse button is currently held. */
        [[nodiscard]] static bool IsMouseButtonHeld(const Uint8 &Button = LEFT_MOUSE_BUTTON);

        /** Returns whether the given mouse button is currently released. */
        [[nodiscard]] static bool IsMouseButtonReleased(const Uint8 &Button = LEFT_MOUSE_BUTTON);

        // MOUSE WHEEL

        /** Returns the number of scrolls for the mouse scroll wheel since the last update on the horizontal axis. */
        [[nodiscard]] static Sint32 GetMouseWheelX();

        /** Returns the number of scrolls for the mouse scroll wheel since the last update on the horizontal axis. */
        [[nodiscard]] static Sint32 GetMouseWheelY();

        // MOUSE POSITION

        /** Returns the mouse's current position. */
        [[nodiscard]] static glm::ivec2 GetMousePosition();

        /** Returns the mouse's current change in position. */
        [[nodiscard]] static glm::ivec2 GetMouseDelta();

        /** Returns the mouse's current change in position normalized. */
        [[nodiscard]] static glm::vec2 GetMouseDeltaNormalized();

        /** Shows the mouse cursor. */
        static void ShowMouse();

        /** Hide the mouse cursor. */
        static void HideMouse();

        /** Returns whether the mouse cursor is currently visible. */
        [[nodiscard]] static bool IsMouseVisible();

        // DELTA TIME

        /** Returns the time point the program started. */
        [[nodiscard]] static std::chrono::high_resolution_clock::time_point StartTime();

        /** Returns the current time point as of this input update. */
        [[nodiscard]] static std::chrono::high_resolution_clock::time_point CurrentTime();

        /** The time in seconds since input was last updated. */
        [[nodiscard]] static double DeltaTime();

        /** Returns the total number of times Update() has been called. */
        [[nodiscard]] static unsigned long long FrameCount();

        /** Returns the elapsed time (in seconds) since the program started as of this input update. */
        [[nodiscard]] static double ElapsedTime();

        /** Returns the current frame rate as of this input update. */
        [[nodiscard]] static double FrameRate();
    };
}
