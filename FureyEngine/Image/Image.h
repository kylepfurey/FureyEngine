// .h
// OpenGL + SDL Image Script
// by Kyle Furey

#pragma once
#include "../Standard/Standard.h"

namespace FureyEngine {
    /** Represents an SDL image that can be rendered on a quad in OpenGL. */
    class Image final {
        // PATH

        /** The path to this image. */
        const std::string ImagePath;

        // SIZE

        /** The size in pixels of this image. */
        glm::ivec2 MyTextureSize = {0, 0};

        // COLORS

        /** Each pixel's color in this image, starting from the top left of the texture. */
        SDL_Color **MyPixels;

        // TEXTURE ID

        /** The ID of the texture OpenGL creates. */
        GLuint MyTextureID = 0;

        // TOTAL IMAGES

        /** The current number of images open. */
        static int TotalImages;

    public:
        // CONSTRUCTORS

        /** Creates a new image from the given path that can be rendered to a window. */
        explicit Image(const std::string &Path);

        /** Creates a new image of the given size and color. */
        explicit Image(const glm::ivec2 &Size, const SDL_Color &Color);

        /** Delete copy constructor. */
        Image(const Image &Copied) = delete;

        /** Delete move constructor. */
        Image(Image &&Moved) = delete;

        // ASSIGNMENT OPERATORS

        /** Delete copy constructor. */
        Image &operator=(const Image &Copied) = delete;

        /** Delete move constructor. */
        Image &operator=(Image &&Moved) = delete;

        // DESTRUCTOR

        /** Properly destroys this image. */
        ~Image();

        // GETTERS

        /** Returns the path to this image's texture. */
        [[nodiscard]] std::string Path() const;

        /** Returns the size in pixels of this image's texture. */
        [[nodiscard]] glm::ivec2 TextureSize() const;

        /** Returns the ID of this texture in OpenGL. */
        [[nodiscard]] GLuint TextureID() const;

        /**
         * Returns a 2D array of each of this image's pixel colors or nullptr if there are none.<br/>
         * The color at [0][0] will always be the top left.
         */
        [[nodiscard]] SDL_Color const *const*Pixels() const;

        /** Returns the total number of images currently active. */
        [[nodiscard]] static int Total();
    };
}
