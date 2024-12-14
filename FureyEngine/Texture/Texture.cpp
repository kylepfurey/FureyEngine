// .cpp
// SDL + OpenGL Texture Structure Script
// by Kyle Furey

#include "Texture.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Constructs a new texture from a texture ID and a transform.
    Texture::Texture(const GLuint &TextureID, const glm::ivec2 &Size, FureyEngine::Transform *Transform)
        : ID(TextureID), Size(Size), Transform(Transform) {
    }
}
