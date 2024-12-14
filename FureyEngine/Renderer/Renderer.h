// .h
// SDL + OpenGL Renderer Class Script
// by Kyle Furey

#pragma once
#include "../Window/Window.h"
#include "../Shader/Shader.h"
#include "../Quad/Quad.h"
#include "../Texture/Texture.h"

#define SCREEN_WIDTH 100
#define UNIFORM_TEXTURE_PARAMETER "texture1"
#define UNIFORM_MODEL_PARAMETER "model"
#define UNIFORM_PROJECTION_PARAMETER "projection"

namespace FureyEngine {
    /** Renders textures to a window with a shader. */
    class Renderer final {
        // RENDERING

        /** Stores information about the vertex array. */
        GLuint VertexArray = 0;

        /** Stores information about the vertex buffer. */
        GLuint VertexBuffer = 0;

        /** Stores information about the element buffer. */
        GLuint ElementBuffer = 0;

        // MATRIX

        /** Stores the texture parameter. */
        GLint TextureParameter = 0;

        /** Stores the uniform model matrix parameter. */
        GLint ModelParameter = 0;

        /** Stores the uniform projection matrix parameter. */
        GLint ProjectionParameter = 0;

        // TOTAL RENDERERS

        /** The total number of renderers open. */
        static int TotalRenderers;

        // RENDERING

        /** Draws the given texture on the screen if it is visible. */
        void DrawTexture(const Texture *Texture) const;

    public:
        // RENDERER

        /** The shader to use with rendering. */
        Shader *Shader;

        /** Each window to render on. */
        std::set<Window *> Windows;

        /** The current position of this renderer. */
        glm::vec2 Position = {0, 0};

        // TEXTURES

        /** A texture rendered to the background. */
        GLuint Background = 0;

        /**
         * A linked list of textures that intend on transforming relative to the renderer's position.<br/>
         * These textures are always rendered first, in order.
         */
        std::list<Texture *> DynamicTextures;

        /**
         * A linked list of textures that remain on the screen.<br/>
         * These textures are always rendered last, in order.
         */
        std::list<Texture *> StaticTextures;

        // CONSTRUCTORS

        /** Creates a new renderer with the given shader and window. */
        Renderer(FureyEngine::Shader *Shader, Window *Window);

        /** Creates a new renderer with the given shader for the given windows. */
        Renderer(FureyEngine::Shader *Shader, const std::initializer_list<Window *> &Windows);

        /** Delete copy constructor. */
        Renderer(const Renderer &Copied) = delete;

        /** Delete move constructor. */
        Renderer(Renderer &&Moved) = delete;

        // ASSIGNMENT OPERATORS

        /** Delete copy constructor. */
        Renderer &operator=(const Renderer &Copied) = delete;

        /** Delete move constructor. */
        Renderer &operator=(Renderer &&Moved) = delete;

        // DESTRUCTOR

        /** Properly destroys this renderer. */
        ~Renderer();

        // GETTERS

        /** Returns the ID of the vertex array object. */
        [[nodiscard]] GLuint VAO() const;

        /** Returns the ID of the vertex buffer object. */
        [[nodiscard]] GLuint VBO() const;

        /** Returns the ID of the element buffer object. */
        [[nodiscard]] GLuint EBO() const;

        /** Returns the ID of the uniform texture parameter. */
        [[nodiscard]] GLint UniformTexture() const;

        /** Returns the ID of the uniform model matrix parameter. */
        [[nodiscard]] GLint UniformModel() const;

        /** Returns the ID of the uniform projection matrix parameter. */
        [[nodiscard]] GLint UniformProjection() const;

        /** Returns the total number of renderers open. */
        [[nodiscard]] static int Total();

        // RENDERING

        /** Renders this renderer's textures relative to its current position. */
        void Render() const;

        /** Clears the screen. */
        void Clear() const;

        /** Clears the screen with the given color. */
        void Clear(const SDL_Color &ClearColor) const;

        /** Returns whether the given texture is currently culled from view. */
        [[nodiscard]] bool IsTextureCulled(const Texture *Texture) const;

        /** Converts the given pixel size into world units. */
        [[nodiscard]] static glm::vec2 PixelsToWorld(const glm::ivec2 &Pixels);

        /** Converts the given world size into pixels. */
        [[nodiscard]] static glm::ivec2 WorldToPixels(const glm::vec2 &Scale);
    };
}
