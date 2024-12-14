// .h
// OpenGL + SDL TTF Font Script
// by Kyle Furey

#include "Font.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Creates a new .ttf font file from the given path.
    // This font can be initialized with parameters to avoid regenerating text.
    Font::Font(const std::string &Path,
               const std::string &Text,
               const int &Size,
               const SDL_Color &Color)
        : FontPath(Path), FontSize(Size), FontColor(Color), FontText(Text) {
        // Initialize SDL_ttf
        if (TotalFonts == 0) {
            if (TTF_Init() == -1) {
                throw std::runtime_error("ERROR: TTF failed to initialize!\nTTF Error: " +
                                         std::string(TTF_GetError()));
            }
        }

        ++TotalFonts;

        // Load the .ttf font file at the given path
        MyFont = TTF_OpenFont(Path.c_str(), Size);
        if (MyFont == nullptr) {
            TTF_Quit();
            throw std::runtime_error("ERROR: TTF failed to load a font file!\nTTF Error: " +
                                     std::string(TTF_GetError()));
        }

        // Create a temporary surface with the given font file
        SDL_Surface *Surface;
#if BLENDED_TEXT
        Surface = TTF_RenderText_Blended(MyFont, FontText.empty() ? " " : FontText.c_str(), Color);
#else
        Surface = TTF_RenderText_Solid(MyFont, FontText.empty() ? " " : FontText.c_str(), Color);
#endif
        if (Surface == nullptr) {
            TTF_CloseFont(MyFont);
            TTF_Quit();
            throw std::runtime_error("ERROR: TTF failed to load a font file!\nTTF Error: " +
                                     std::string(TTF_GetError()));
        }

        // Generates a new texture and stores its ID
        glGenTextures(1, &MyTextureID);
        if (MyTextureID == 0) {
            TTF_CloseFont(MyFont);
            TTF_Quit();
            throw std::runtime_error("ERROR: OpenGL failed to generate a texture!\nOpenGL Error: " +
                                     std::to_string(glGetError()));
        }

        // Bind the texture
        glBindTexture(GL_TEXTURE_2D, MyTextureID);

        // Ensures the texture wraps and is interpolated across the screen
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Reformats the surface
        SDL_Surface *FormattedSurface = SDL_ConvertSurfaceFormat(Surface, SDL_PIXELFORMAT_RGBA32, 0);

        // Set the size of this font file's texture
        MyTextureSize = {Surface->w, Surface->h};

        // Stores the texture data in the GPU and frees the surface
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FormattedSurface->w, FormattedSurface->h,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, FormattedSurface->pixels);
        SDL_FreeSurface(FormattedSurface);
        SDL_FreeSurface(Surface);

        // Unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // DESTRUCTOR

    // Properly destroys this font file.
    Font::~Font() {
        // Destroys the font file
        if (MyFont != nullptr) {
            TTF_CloseFont(MyFont);
            MyFont = nullptr;
        }

        // Destroys the texture in OpenGL
        if (MyTextureID != 0) {
            glDeleteTextures(1, &MyTextureID);
            MyTextureID = 0;
        }

        --TotalFonts;

        // Close SDL_ttf if there are no more font files remaining
        if (TotalFonts == 0) {
            TTF_Quit();
        }
    }

    // GETTERS

    // Returns the path to this font file's texture.
    std::string Font::Path() const {
        return FontPath;
    }

    // Returns the current size of this font file.
    int Font::Size() const {
        return FontSize;
    }

    // Returns the current color of this font file.
    SDL_Color Font::Color() const {
        return FontColor;
    }

    // Returns the current text of this font file.
    std::string Font::Text() const {
        return FontText;
    }

    // Returns the pointer to the current TTF_Font of this font file.
    // NOTE: This is updated each time this font file is reinitialized with a new size.
    TTF_Font *Font::TTF_Font() const {
        return MyFont;
    }

    // Returns the current size of this font file's texture.
    // NOTE: This is updated each time this font file is reinitialized.
    glm::ivec2 Font::TextureSize() const {
        return MyTextureSize;
    }

    // Returns the ID of this font file's current texture in OpenGL.
    // NOTE: This is updated each time this font file is reinitialized.
    GLuint Font::TextureID() const {
        return MyTextureID;
    }

    // Returns the total number of font files currently active.
    int Font::Total() {
        return TotalFonts;
    }

    // REINITIALIZATION

    // Reinitializes this font with the given parameters.
    // Order of parameters: std::string Text, int Size, SDL_Color Color.
    // Returns the newly created texture ID.
    GLuint Font::Reinitialize(const std::string &Text) {
        return Reinitialize(Text, FontSize, FontColor);
    }

    // Reinitializes this font with the given parameters.
    // Order of parameters: std::string Text, int Size, SDL_Color Color.
    // Returns the newly created texture ID.
    GLuint Font::Reinitialize(const int &Size) {
        return Reinitialize(FontText, Size, FontColor);
    }

    // Reinitializes this font with the given parameters.
    // Order of parameters: std::string Text, int Size, SDL_Color Color.
    // Returns the newly created texture ID.
    GLuint Font::Reinitialize(const SDL_Color &Color) {
        return Reinitialize(FontText, FontSize, Color);
    }

    // Reinitializes this font with the given parameters.
    // Order of parameters: std::string Text, int Size, SDL_Color Color.
    // Returns the newly created texture ID.
    GLuint Font::Reinitialize(const std::string &Text, const int &Size) {
        return Reinitialize(Text, Size, FontColor);
    }

    // Reinitializes this font with the given parameters.
    // Order of parameters: std::string Text, int Size, SDL_Color Color.
    // Returns the newly created texture ID.
    GLuint Font::Reinitialize(const std::string &Text, const SDL_Color &Color) {
        return Reinitialize(Text, FontSize, Color);
    }

    // Reinitializes this font with the given parameters.
    // Order of parameters: std::string Text, int Size, SDL_Color Color.
    // Returns the newly created texture ID.
    GLuint Font::Reinitialize(const int &Size, const SDL_Color &Color) {
        return Reinitialize(FontText, Size, Color);
    }

    // Reinitializes this font with the given parameters.
    // Order of parameters: std::string Text, int Size, SDL_Color Color.
    // Returns the newly created texture ID.
    GLuint Font::Reinitialize(const std::string &Text, const int &Size, const SDL_Color &Color, const bool &Force) {
        // Make sure reinitialization is necessary
        if (!Force && FontText == Text && FontSize == Size &&
            FontColor.r == Color.r && FontColor.g == Color.g && FontColor.b == Color.b && FontColor.a == Color.a) {
            return MyTextureID;
        }

        // Destroys the texture in OpenGL
        if (MyTextureID != 0) {
            glDeleteTextures(1, &MyTextureID);
            MyTextureID = 0;
        }

        // Destroys the font file if necessary
        if (FontSize != Size || Force) {
            if (MyFont != nullptr) {
                TTF_CloseFont(MyFont);
            }

            // Load the .ttf font file at the given path
            MyFont = TTF_OpenFont(FontPath.c_str(), Size);
            if (MyFont == nullptr) {
                TTF_Quit();
                throw std::runtime_error("ERROR: TTF failed to reload a font file!\nTTF Error: " +
                                         std::string(TTF_GetError()));
            }
        }

        FontText = Text;

        // Create a temporary surface with the given font file
        SDL_Surface *Surface;
#if BLENDED_TEXT
        Surface = TTF_RenderText_Blended(MyFont, FontText.empty() ? " " : FontText.c_str(), Color);
#else
        Surface = TTF_RenderText_Solid(MyFont, FontText.empty() ? " " : FontText.c_str(), Color);
#endif
        if (Surface == nullptr) {
            TTF_CloseFont(MyFont);
            TTF_Quit();
            throw std::runtime_error("ERROR: TTF failed to reload a font file!\nTTF Error: " +
                                     std::string(TTF_GetError()));
        }

        // Generates a new texture and stores its ID
        glGenTextures(1, &MyTextureID);
        if (MyTextureID == 0) {
            TTF_CloseFont(MyFont);
            TTF_Quit();
            throw std::runtime_error("ERROR: OpenGL failed to generate a texture!\nOpenGL Error: " +
                                     std::to_string(glGetError()));
        }

        // Bind the texture
        glBindTexture(GL_TEXTURE_2D, MyTextureID);

        // Ensures the texture wraps and is interpolated across the screen
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Reformats the surface
        SDL_Surface *FormattedSurface = SDL_ConvertSurfaceFormat(Surface, SDL_PIXELFORMAT_RGBA32, 0);

        // Set the size of this font file's texture
        MyTextureSize = {Surface->w, Surface->h};

        // Stores the texture data in the GPU and frees the surface
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FormattedSurface->w, FormattedSurface->h,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, FormattedSurface->pixels);
        SDL_FreeSurface(FormattedSurface);
        SDL_FreeSurface(Surface);

        // Unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);

        return MyTextureID;
    }

    // STATIC VARIABLE INITIALIZATION

    // The current number of font files open.
    int Font::TotalFonts = 0;
}
