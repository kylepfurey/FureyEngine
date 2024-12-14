// .h
// OpenGL + SDL TTF Font Script
// by Kyle Furey

#pragma once
#include "../Standard/Standard.h"

#define BLENDED_TEXT 1

namespace FureyEngine {
    /** Represents an SDL TTF font that can be used to render text on a quad in OpenGL. */
    class Font final {
        // PATH

        /** The path to the font file. */
        const std::string FontPath;

        // FONT

        /** The current size of this font file. */
        int FontSize;

        /** The current SDL color of this font file. */
        SDL_Color FontColor;

        /** The current text rendered by this font file.*/
        std::string FontText;

        /** The TTF font of this font file. */
        TTF_Font *MyFont;

        // SIZE

        /** The current size in pixels of this font file's texture. */
        glm::ivec2 MyTextureSize = {0, 0};

        // TEXTURE ID

        /** The ID of the texture OpenGL creates. */
        GLuint MyTextureID = 0;

        // TOTAL FONTS

        /** The current number of font files created. */
        static int TotalFonts;

    public:
        // CONSTRUCTORS

        /**
         * Creates a new .ttf font file from the given path.<br/>
         * This font can be initialized with parameters to avoid regenerating text.
         */
        explicit Font(const std::string &Path,
                      const std::string &Text = "",
                      const int &Size = 32,
                      const SDL_Color &Color = {255, 255, 255, 255});

        /** Delete copy constructor. */
        Font(const Font &Copied) = delete;

        /** Delete move constructor. */
        Font(Font &&Moved) = delete;

        // ASSIGNMENT OPERATORS

        /** Delete copy constructor. */
        Font &operator=(const Font &Copied) = delete;

        /** Delete move constructor. */
        Font &operator=(Font &&Moved) = delete;

        // DESTRUCTOR

        /** Properly destroys this font file. */
        ~Font();

        // GETTERS

        /** Returns the path to this font file's texture. */
        [[nodiscard]] std::string Path() const;

        /** Returns the current size of this font file. */
        [[nodiscard]] int Size() const;

        /** Returns the current color of this font file. */
        [[nodiscard]] SDL_Color Color() const;

        /** Returns the current text of this font file. */
        [[nodiscard]] std::string Text() const;

        /**
         * Returns the pointer to the current TTF_Font of this font file.<br/>
         * NOTE: This is updated each time this font file is reinitialized with a new size.
         */
        [[nodiscard]] TTF_Font *TTF_Font() const;

        /**
         * Returns the current size of this font file's texture.<br/>
         * NOTE: This is updated each time this font file is reinitialized.
         */
        [[nodiscard]] glm::ivec2 TextureSize() const;

        /**
         * Returns the ID of this font file's current texture in OpenGL.<br/>
         * NOTE: This is updated each time this font file is reinitialized.
         */
        [[nodiscard]] GLuint TextureID() const;

        /** Returns the total number of font files currently active. */
        [[nodiscard]] static int Total();

        // REINITIALIZATION

        /**
         * Reinitializes this font with the given parameters.<br/>
         * Order of parameters: std::string Text, int Size, SDL_Color Color.<br/>
         * Returns the newly created texture ID.
         */
        GLuint Reinitialize(const std::string &Text);

        /**
         * Reinitializes this font with the given parameters.<br/>
         * Order of parameters: std::string Text, int Size, SDL_Color Color.<br/>
         * Returns the newly created texture ID.
         */
        GLuint Reinitialize(const int &Size);

        /**
         * Reinitializes this font with the given parameters.<br/>
         * Order of parameters: std::string Text, int Size, SDL_Color Color.<br/>
         * Returns the newly created texture ID.
         */
        GLuint Reinitialize(const SDL_Color &Color);

        /**
         * Reinitializes this font with the given parameters.<br/>
         * Order of parameters: std::string Text, int Size, SDL_Color Color.<br/>
         * Returns the newly created texture ID.
         */
        GLuint Reinitialize(const std::string &Text,
                            const int &Size);

        /**
         * Reinitializes this font with the given parameters.<br/>
         * Order of parameters: std::string Text, int Size, SDL_Color Color.<br/>
         * Returns the newly created texture ID.
         */
        GLuint Reinitialize(const std::string &Text,
                            const SDL_Color &Color);

        /**
         * Reinitializes this font with the given parameters.<br/>
         * Order of parameters: std::string Text, int Size, SDL_Color Color.<br/>
         * Returns the newly created texture ID.
         */
        GLuint Reinitialize(const int &Size,
                            const SDL_Color &Color);

        /**
         * Reinitializes this font with the given parameters.<br/>
         * Order of parameters: std::string Text, int Size, SDL_Color Color.<br/>
         * Returns the newly created texture ID.
         */
        GLuint Reinitialize(const std::string &Text,
                            const int &Size,
                            const SDL_Color &Color,
                            const bool &Force = false);
    };
}
