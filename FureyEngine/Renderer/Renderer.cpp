// .cpp
// SDL + OpenGL Renderer Class Script
// by Kyle Furey

#include "Renderer.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Creates a new renderer with the given shader and window.
    Renderer::Renderer(FureyEngine::Shader *Shader, Window *Window) : Shader(Shader) {
        if (Shader == nullptr || Window == nullptr) {
            throw std::runtime_error("ERROR: Attempting to render with a null shader or window.");
        }

        ++TotalRenderers;

        this->Windows.insert(Window);

        // Store the vertex array and vertex buffer
        glGenVertexArrays(1, &VertexArray);
        glBindVertexArray(VertexArray);
        glGenBuffers(1, &VertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Quad::Vertices), Quad::Vertices, GL_STATIC_DRAW);
        glGenBuffers(1, &ElementBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Quad::Indices), Quad::Indices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void *>(nullptr));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // Store the model and projection matrix locations
        TextureParameter = glGetUniformLocation(Shader->ProgramID(), UNIFORM_TEXTURE_PARAMETER);
        ModelParameter = glGetUniformLocation(Shader->ProgramID(), UNIFORM_MODEL_PARAMETER);
        ProjectionParameter = glGetUniformLocation(Shader->ProgramID(), UNIFORM_PROJECTION_PARAMETER);
        if (const GLenum Error = glGetError(); Error != GL_NO_ERROR) {
            throw std::runtime_error("ERROR: OpenGL failed to initialize a renderer!\nOpenGL Error: " +
                                     std::to_string(Error));
        }
    }

    // Creates a new renderer with the given shader for the given windows.
    Renderer::Renderer(FureyEngine::Shader *Shader, const std::initializer_list<Window *> &Windows)
        : Shader(Shader) {
        if (Shader == nullptr) {
            throw std::runtime_error("ERROR: Attempting to render with a null shader or window.");
        }

        ++TotalRenderers;

        for (const auto Window: Windows) {
            if (Window == nullptr) {
                throw std::runtime_error("ERROR: Attempting to render with a null shader or window.");
            }

            this->Windows.insert(Window);
        }

        // Store the vertex array and vertex buffer
        glGenVertexArrays(1, &VertexArray);
        glBindVertexArray(VertexArray);
        glGenBuffers(1, &VertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Quad::Vertices), Quad::Vertices, GL_STATIC_DRAW);
        glGenBuffers(1, &ElementBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Quad::Indices), Quad::Indices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void *>(nullptr));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // Store the model and projection matrix locations
        TextureParameter = glGetUniformLocation(Shader->ProgramID(), UNIFORM_TEXTURE_PARAMETER);
        ModelParameter = glGetUniformLocation(Shader->ProgramID(), UNIFORM_MODEL_PARAMETER);
        ProjectionParameter = glGetUniformLocation(Shader->ProgramID(), UNIFORM_PROJECTION_PARAMETER);
        if (const GLenum Error = glGetError(); Error != GL_NO_ERROR) {
            throw std::runtime_error("ERROR: OpenGL failed to initialize a renderer!\nOpenGL Error: " +
                                     std::to_string(Error));
        }
    }

    // DESTRUCTOR

    // Properly destroys this renderer.
    Renderer::~Renderer() {
        // Deletes the vertex array and buffers
        glDeleteBuffers(1, &ElementBuffer);
        glDeleteBuffers(1, &VertexBuffer);
        glDeleteVertexArrays(1, &VertexArray);

        --TotalRenderers;
    }

    // GETTERS

    // Returns the ID of the vertex array object.
    GLuint Renderer::VAO() const {
        return VertexArray;
    }

    // Returns the ID of the vertex buffer object.
    GLuint Renderer::VBO() const {
        return VertexBuffer;
    }

    // Returns the ID of the element buffer object.
    GLuint Renderer::EBO() const {
        return ElementBuffer;
    }

    // Returns the ID of the uniform texture parameter.
    GLint Renderer::UniformTexture() const {
        return TextureParameter;
    }

    // Returns the ID of the uniform model matrix parameter.
    GLint Renderer::UniformModel() const {
        return ModelParameter;
    }

    // Returns the ID of the uniform projection matrix parameter.
    GLint Renderer::UniformProjection() const {
        return ProjectionParameter;
    }

    // Returns the total number of renderers open.
    int Renderer::Total() {
        return TotalRenderers;
    }

    // RENDERING

    // Draws the given texture on the screen if it is visible.
    void Renderer::DrawTexture(const Texture *Texture) const {
        if (Texture->ID == 0) {
            throw std::runtime_error("ERROR: Attempting to draw an empty texture!");
        }

        if (Texture->Transform == nullptr) {
            throw std::runtime_error("ERROR: Attempting to draw a texture with a null transform!");
        }

        if (Texture->Size.x == 0 || Texture->Size.y == 0) {
            return;
        }

        // Cull the texture if the renderer cannot currently see it
        if (IsTextureCulled(Texture)) {
            // std::cout << "Culled a texture!" << std::endl;
            return;
        }

        // Copy the transform
        Transform Transform = *Texture->Transform;

        // Adjust the position relative to this renderer's location and resolution
        Transform.Position.x -= Position.x;
        Transform.Position.y -= Position.y;

        // Convert pixels to screen units based on the set SCREEN_WIDTH
        Transform.Position.x /= static_cast<float>(TARGET_RESOLUTION_HEIGHT) / 2
                / (static_cast<float>(TARGET_RESOLUTION_WIDTH) / SCREEN_WIDTH);
        Transform.Position.y /= -static_cast<float>(TARGET_RESOLUTION_HEIGHT) / 2
                / (static_cast<float>(TARGET_RESOLUTION_WIDTH) / SCREEN_WIDTH);

        // Convert the rotation from degrees to radians
        Transform.Rotation = glm::radians(Transform.Rotation);

        // Adjust the scale of the texture relative to the window
        Transform.Scale.x *= static_cast<float>(Texture->Size.x) / static_cast<float>(TARGET_RESOLUTION_HEIGHT);
        Transform.Scale.y *= static_cast<float>(Texture->Size.y) / static_cast<float>(TARGET_RESOLUTION_HEIGHT);

        // Send the model matrix to the shader
        const glm::mat4 ModelMatrix = Transform.GetMatrix();
        glUniformMatrix4fv(ModelParameter, 1, GL_FALSE, &ModelMatrix[0][0]);

        // Render the texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture->ID);
        glBindVertexArray(VertexArray);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    // Renders this renderer's textures relative to its current position.
    void Renderer::Render() const {
        // Render for each window
        for (const auto Window: Windows) {
            // Set the current window
            SDL_GL_MakeCurrent(Window->SDL_Window(), Window->SDL_GLContext());

            // Get the scale of the window and make OpenGL match it
            const auto WindowSize = Window->Size();
            glViewport(0, 0, WindowSize.x, WindowSize.y);

            // Render with the shader
            glUseProgram(Shader->ProgramID());

            // Send the projection matrix to the shader
            const glm::mat4 ProjectionMatrix = glm::ortho(-TARGET_RESOLUTION_RATIO, TARGET_RESOLUTION_RATIO,
                                                          1.0f, -1.0f);
            glUniformMatrix4fv(ProjectionParameter, 1, GL_FALSE, &ProjectionMatrix[0][0]);

            // Write to the texture inside the shader
            glUniform1i(TextureParameter, 0);

            // Render the background
            if (Background != 0) {
                Transform BackgroundTransform;

                // Move the background with the renderer
                BackgroundTransform.Position = Position;

                // Send the model matrix to the shader
                const glm::mat4 ModelMatrix = BackgroundTransform.GetMatrix();
                glUniformMatrix4fv(ModelParameter, 1, GL_FALSE, &ModelMatrix[0][0]);

                // Render the texture
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, Background);
                glBindVertexArray(VertexArray);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            }

            // Render each dynamic texture
            for (const auto Texture: DynamicTextures) {
                DrawTexture(Texture);
            }

            // Render each static texture
            for (const auto Texture: StaticTextures) {
                // Move the texture with the renderer
                Texture->Transform->Position.x += Position.x;
                Texture->Transform->Position.y += Position.y;

                DrawTexture(Texture);

                // Move the texture back to its initial position
                Texture->Transform->Position.x -= Position.x;
                Texture->Transform->Position.y -= Position.y;
            }

            // Update the window
            SDL_GL_SwapWindow(Window->SDL_Window());
        }

        // Unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Clears the screen.
    void Renderer::Clear() const {
        // Clear each window
        for (const auto Window: Windows) {
            // Set the current window
            SDL_GL_MakeCurrent(Window->SDL_Window(), Window->SDL_GLContext());

            // Clear the current window
            glClear(GL_COLOR_BUFFER_BIT);
        }
    }

    // Clears the screen with the given color.
    void Renderer::Clear(const SDL_Color &ClearColor) const {
        // Clear each window
        for (const auto Window: Windows) {
            // Set the current window
            SDL_GL_MakeCurrent(Window->SDL_Window(), Window->SDL_GLContext());

            // Set the clear color
            glClearColor(static_cast<GLclampf>(ClearColor.r) / 255.0f,
                         static_cast<GLclampf>(ClearColor.g) / 255.0f,
                         static_cast<GLclampf>(ClearColor.b) / 255.0f,
                         static_cast<GLclampf>(ClearColor.a) / 255.0f);

            // Clear the current window
            glClear(GL_COLOR_BUFFER_BIT);
        }
    }

    // Returns whether the given texture is currently culled from view.
    bool Renderer::IsTextureCulled(const Texture *Texture) const {
        const auto Size =
                (static_cast<float>(Texture->Size.x) * Texture->Transform->Scale.x >=
                 static_cast<float>(Texture->Size.y) * Texture->Transform->Scale.y
                     ? static_cast<float>(Texture->Size.x) * Texture->Transform->Scale.x
                     : static_cast<float>(Texture->Size.y) * Texture->Transform->Scale.y)
                * 0.70710678118 / static_cast<float>(TARGET_RESOLUTION_WIDTH) * SCREEN_WIDTH;

        return Position.x - static_cast<float>(SCREEN_WIDTH) / 2
               > Texture->Transform->Position.x + Size ||
               Position.x + static_cast<float>(SCREEN_WIDTH) / 2
               < Texture->Transform->Position.x - Size ||
               Position.y - SCREEN_WIDTH * (1 / TARGET_RESOLUTION_RATIO) / 2
               > Texture->Transform->Position.y + Size ||
               Position.y + SCREEN_WIDTH * (1 / TARGET_RESOLUTION_RATIO) / 2
               < Texture->Transform->Position.y - Size;
    }

    // Converts the given pixel size into world units.
    glm::vec2 Renderer::PixelsToWorld(const glm::ivec2 &Pixels) {
        glm::vec2 Scale;
        Scale.x = static_cast<float>(Pixels.x) / TARGET_RESOLUTION_WIDTH * SCREEN_WIDTH;
        Scale.y = static_cast<float>(Pixels.y) / TARGET_RESOLUTION_WIDTH * SCREEN_WIDTH;
        return Scale;
    }

    // Converts the given world size into pixels.
    glm::ivec2 Renderer::WorldToPixels(const glm::vec2 &Scale) {
        glm::ivec2 Pixels;
        Pixels.x = static_cast<int>(Scale.x) * TARGET_RESOLUTION_WIDTH / SCREEN_WIDTH;
        Pixels.y = static_cast<int>(Scale.y) * TARGET_RESOLUTION_WIDTH / SCREEN_WIDTH;
        return Pixels;
    }

    // STATIC VARIABLE INITIALIZATION

    // The current number of renderers open.
    int Renderer::TotalRenderers = 0;
}
