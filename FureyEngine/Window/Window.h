// .h
// OpenGL + SDL Window Script
// by Kyle Furey

#pragma once
#include "../Standard/Standard.h"

#define RESIZABLE_WINDOWS 1
#define MOVABLE_WINDOWS 1
#define TARGET_RESOLUTION_WIDTH 1920
#define TARGET_RESOLUTION_HEIGHT 1080
#define TARGET_RESOLUTION_RATIO (static_cast<float>(TARGET_RESOLUTION_WIDTH) / static_cast<float>(TARGET_RESOLUTION_HEIGHT))

#define GL_ERROR_CHECK \
if (const auto Error = glGetError(); Error != GL_NO_ERROR) {\
    throw std::runtime_error(std::string("ERROR: An OpenGL error was thrown!\nOpenGL Error: ") + std::to_string(Error));\
}
#define SDL_ERROR_CHECK \
if (const std::string Error = SDL_GetError(); !Error.empty()) {\
    throw std::runtime_error(std::string("ERROR: An SDL error was thrown!\nSDL Error: ") + Error);\
}
#define TRY_CATCH(CodeBlock) \
    try {\
        CodeBlock\
    } catch (const std::exception &Exception) {\
        std::cerr << Exception.what() << std::endl;\
        std::abort();\
    }

namespace FureyEngine {
    /** Represents an automatically configured OpenGL window created with SDL. */
    class Window final {
        // NAME

        /** The name of this window. */
        std::string WindowName;

        // WINDOW

        /** The SDL window that makes up this window class. */
        SDL_Window *MyWindow;

        // CONTEXT

        /** The OpenGL context that renders to this window class. */
        SDL_GLContext WindowContext;

        // TOTAL WINDOWS

        /** The current number of windows open. */
        static int TotalWindows;

    public:
        // CONSTRUCTORS

        /**
         * Creates a new window with the given settings.<br/>
         * Automatically configures SDL if this is the first window opened.
         */
        explicit Window(const std::string &Name,
                        const int &Width = TARGET_RESOLUTION_WIDTH, const int &Height = TARGET_RESOLUTION_HEIGHT,
                        const int &X = SDL_WINDOWPOS_CENTERED, const int &Y = SDL_WINDOWPOS_CENTERED);

        /** Delete copy constructor. */
        Window(const Window &Copied) = delete;

        /** Delete move constructor. */
        Window(Window &&Moved) = delete;

        // ASSIGNMENT OPERATORS

        /** Delete copy constructor. */
        Window &operator=(const Window &Copied) = delete;

        /** Delete move constructor. */
        Window &operator=(Window &&Moved) = delete;

        // DESTRUCTOR

        /**
         * Properly destroys this window.<br/>
         * Automatically closes SDL if this was the last window open.
         */
        ~Window();

        // GETTERS

        /** Returns the name of this window. */
        [[nodiscard]] std::string Name() const;

        /** Returns the current size in pixels of this window. */
        [[nodiscard]] glm::ivec2 Size() const;

        /** Returns the current position of this window. */
        [[nodiscard]] glm::ivec2 Position() const;

        /** Returns the pointer to the SDL_Window of this window. */
        [[nodiscard]] SDL_Window *SDL_Window() const;

        /** Returns the SDL_GLContext of this window. */
        [[nodiscard]] SDL_GLContext &SDL_GLContext();

        /** Returns the current number of windows open. */
        [[nodiscard]] static int Total();

        // SETTERS

        /** Sets the name of this window. */
        void SetName(const std::string &Name);

        /** Sets the size of this window. */
        void SetSize(const int &Width, const int &Height) const;

        /** Sets the position of this window. */
        void SetPosition(const int &X, const int &Y) const;
    };
}
