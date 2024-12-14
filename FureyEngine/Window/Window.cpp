// .cpp
// OpenGL + SDL Window Script
// by Kyle Furey

#include "Window.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Creates a new window with the given settings.
    // Automatically configures SDL if this is the first window opened.
    Window::Window(const std::string &Name, const int &Width, const int &Height, const int &X, const int &Y)
        : WindowName(Name) {
        // Initialize SDL
        if (TotalWindows == 0) {
            if (SDL_Init(SDL_INIT_EVERYTHING & ~SDL_INIT_SENSOR) != 0) {
                throw std::runtime_error("ERROR: SDL failed to initialize!\nSDL Error: " +
                                         std::string(SDL_GetError()));
            }

            // Initialize OpenGL
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        }

        ++TotalWindows;

        // Create a window
        MyWindow = SDL_CreateWindow(Name.c_str(),
                                    X, Y,
                                    Width, Height,
                                    SDL_WINDOW_SHOWN |
                                    SDL_WINDOW_OPENGL |
                                    (RESIZABLE_WINDOWS ? SDL_WINDOW_RESIZABLE : 0) |
                                    (MOVABLE_WINDOWS ? 0 : SDL_WINDOW_BORDERLESS));
        if (MyWindow == nullptr) {
            SDL_Quit();
            throw std::runtime_error("ERROR: SDL failed to create a window!\nSDL Error: " +
                                     std::string(SDL_GetError()));
        }

        // Create an OpenGL context
        WindowContext = SDL_GL_CreateContext(MyWindow);
        if (WindowContext == nullptr) {
            SDL_DestroyWindow(MyWindow);
            SDL_Quit();
            throw std::runtime_error("ERROR: SDL failed to create an OpenGL context!\nSDL Error: " +
                                     std::string(SDL_GetError()));
        }

        // Initialize GLEW
        glewExperimental = GL_TRUE;
        if (const GLenum Result = glewInit(); Result != GLEW_OK) {
            SDL_DestroyWindow(MyWindow);
            SDL_Quit();
            throw std::runtime_error("ERROR: GLEW failed to initialize!\nGLEW Error: " +
                                     std::string(reinterpret_cast<const char *>(glewGetErrorString(Result))));
        }

        // Verify OpenGL initialized successfully
        if (glGetError() != GL_NO_ERROR) {
            SDL_DestroyWindow(MyWindow);
            SDL_Quit();
            throw std::runtime_error("ERROR: OpenGL failed to initialize!\nOpenGL Error: " +
                                     std::to_string(glGetError()));
        }

        // Render the screen
        glViewport(0, 0, Width, Height);
        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(MyWindow);
    }

    // DESTRUCTOR

    // Properly destroys this window.
    // Automatically closes SDL if this was the last window open.
    Window::~Window() {
        // Clean up the window and its context
        if (WindowContext != nullptr) {
            SDL_GL_DeleteContext(WindowContext);
            WindowContext = nullptr;
        }
        if (MyWindow != nullptr) {
            SDL_DestroyWindow(MyWindow);
            MyWindow = nullptr;
        }

        --TotalWindows;

        // Close SDL if there are no more windows remaining
        if (TotalWindows == 0) {
            SDL_Quit();
        }
    }

    // GETTERS

    // Returns the name of this window.
    std::string Window::Name() const {
        return WindowName;
    }

    // Returns the current size in pixels of this window.
    glm::ivec2 Window::Size() const {
        glm::ivec2 Size;
        SDL_GetWindowSize(MyWindow, &Size.x, &Size.y);
        return Size;
    }

    // Returns the current position of this window.
    glm::ivec2 Window::Position() const {
        glm::ivec2 Position;
        SDL_GetWindowPosition(MyWindow, &Position.x, &Position.y);
        return Position;
    }

    // Returns the pointer to the SDL_Window of this window.
    SDL_Window *Window::SDL_Window() const {
        return MyWindow;
    }

    // Returns the SDL_GLContext of this window.
    SDL_GLContext &Window::SDL_GLContext() {
        return WindowContext;
    }

    // Returns the current number of windows open.
    int Window::Total() {
        return TotalWindows;
    }

    // SETTERS

    // Sets the name of this window.
    void Window::SetName(const std::string &Name) {
        WindowName = Name;
        SDL_SetWindowTitle(MyWindow, Name.c_str());
    }

    // Sets the size of this window.
    void Window::SetSize(const int &Width, const int &Height) const {
        SDL_SetWindowSize(MyWindow, Width, Height);
    }

    // Sets the position of this window.
    void Window::SetPosition(const int &X, const int &Y) const {
        SDL_SetWindowPosition(MyWindow, X, Y);
    }

    // STATIC VARIABLE INITIALIZATION

    // The current number of windows open.
    int Window::TotalWindows = 0;
}
