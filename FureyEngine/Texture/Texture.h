// .h
// SDL + OpenGL Texture Structure Script
// by Kyle Furey

#pragma once
#include "../Transform/Transform.h"

namespace FureyEngine {
    /** Represents how a texture should be rendered on the screen. */
    enum class RenderMode {
        /** This texture will move around the screen and should be rendered last. */
        DYNAMIC_TEXTURE = 0,

        /** This texture will not move around the screen and should be rendered first. */
        STATIC_TEXTURE = 1
    };

    /** Represents a single texture that can be rendered with a transform. */
    struct Texture {
        // TEXTURE ID

        /** The ID of this texture. */
        GLuint ID = 0;

        // TEXTURE TRANSFORM

        /** The size in pixels of this texture. */
        glm::ivec2 Size = {0, 0};

        /** A pointer to the transform of this texture. */
        Transform *Transform = nullptr;

        // TEXTURE CONSTRUCTOR

        /** Constructs a new texture from a texture ID and a transform. */
        Texture(const GLuint &TextureID, const glm::ivec2 &Size, FureyEngine::Transform *Transform);
    };
}
