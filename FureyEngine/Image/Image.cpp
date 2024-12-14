// .cpp
// OpenGL + SDL Image Script
// by Kyle Furey

#include "Image.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Creates a new image from the given path that can be rendered to a window.
    Image::Image(const std::string &Path) : ImagePath(Path) {
        // Initialize SDL_image
        if (TotalImages == 0) {
            if (IMG_Init(IMG_INIT_PNG) == 0) {
                throw std::runtime_error("ERROR: IMG failed to initialize!\nIMG Error: " +
                                         std::string(IMG_GetError()));
            }
        }

        ++TotalImages;

        // Create a temporary surface with the given image
        SDL_Surface *Surface = IMG_Load(Path.c_str());
        if (Surface == nullptr) {
            IMG_Quit();
            throw std::runtime_error("ERROR: IMG failed to load an image!\nIMG Error: " +
                                     std::string(IMG_GetError()));
        }

        // Generates a new texture and stores its ID
        glGenTextures(1, &MyTextureID);
        if (MyTextureID == 0) {
            IMG_Quit();
            throw std::runtime_error("ERROR: OpenGL failed to generate a texture!\nOpenGL Error: " +
                                     std::to_string(glGetError()));
        }

        glBindTexture(GL_TEXTURE_2D, MyTextureID);

        // Ensures the texture wraps and is interpolated across the screen
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Reformats the surface
        SDL_Surface *FormattedSurface = SDL_ConvertSurfaceFormat(Surface, SDL_PIXELFORMAT_RGBA32, 0);

        // Set the size of this image's texture
        MyTextureSize = {Surface->w, Surface->h};

        // Store the pixel colors of this image
        if (MyTextureSize.x == 0 || MyTextureSize.y == 0) {
            MyPixels = nullptr;
        } else {
            MyPixels = new SDL_Color *[MyTextureSize.x];
            const auto Pixels = static_cast<Uint32 *>(FormattedSurface->pixels);
            for (int x = 0; x < MyTextureSize.x; ++x) {
                MyPixels[x] = new SDL_Color[MyTextureSize.y];
            }

            for (int y = 0; y < MyTextureSize.y; ++y) {
                for (int x = 0; x < MyTextureSize.x; ++x) {
                    SDL_Color Color;
                    SDL_GetRGBA(Pixels[y * FormattedSurface->w + x], FormattedSurface->format,
                                &Color.r, &Color.g, &Color.b, &Color.a);
                    MyPixels[x][y] = Color;
                }
            }
        }

        // Stores the texture data in the GPU and frees the surface
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FormattedSurface->w, FormattedSurface->h,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, FormattedSurface->pixels);
        SDL_FreeSurface(FormattedSurface);
        SDL_FreeSurface(Surface);

        // Unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Creates a new image of the given size and color.
    Image::Image(const glm::ivec2 &Size, const SDL_Color &Color) : ImagePath("NULL") {
        if (Size.x < 0 || Size.y < 0) {
            throw std::runtime_error("ERROR: Cannot create an image of negative size!");
        }

        // Initialize SDL_image
        if (TotalImages == 0) {
            if (IMG_Init(IMG_INIT_PNG) == 0) {
                throw std::runtime_error("ERROR: IMG failed to initialize!\nIMG Error: " +
                                         std::string(IMG_GetError()));
            }
        }

        ++TotalImages;

        // Create a temporary surface with the given size and color
        SDL_Surface *Surface = SDL_CreateRGBSurface(0, Size.x, Size.y, 32,
                                                    0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
        if (Surface == nullptr) {
            IMG_Quit();
            throw std::runtime_error("ERROR: IMG failed to load an image!\nIMG Error: " +
                                     std::string(IMG_GetError()));
        }

        // Fill the image with a color
        const auto RectColor = SDL_MapRGBA(Surface->format, Color.r, Color.g, Color.b, Color.a);
        SDL_FillRect(Surface, nullptr, RectColor);

        // Generates a new texture and stores its ID
        glGenTextures(1, &MyTextureID);
        if (MyTextureID == 0) {
            IMG_Quit();
            throw std::runtime_error("ERROR: OpenGL failed to generate a texture!\nOpenGL Error: " +
                                     std::to_string(glGetError()));
        }

        glBindTexture(GL_TEXTURE_2D, MyTextureID);

        // Ensures the texture wraps and is interpolated across the screen
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Reformats the surface
        SDL_Surface *FormattedSurface = SDL_ConvertSurfaceFormat(Surface, SDL_PIXELFORMAT_RGBA32, 0);

        // Set the size of this image's texture
        MyTextureSize = {Surface->w, Surface->h};

        // Store the pixel colors of this image
        if (MyTextureSize.x == 0 || MyTextureSize.y == 0) {
            MyPixels = nullptr;
        } else {
            MyPixels = new SDL_Color *[MyTextureSize.x];
            for (int x = 0; x < MyTextureSize.x; ++x) {
                MyPixels[x] = new SDL_Color[MyTextureSize.y];
            }

            for (int y = 0; y < MyTextureSize.y; ++y) {
                for (int x = 0; x < MyTextureSize.x; ++x) {
                    MyPixels[x][y] = Color;
                }
            }
        }

        // Stores the texture data in the GPU and frees the surface
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FormattedSurface->w, FormattedSurface->h,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, FormattedSurface->pixels);
        SDL_FreeSurface(FormattedSurface);
        SDL_FreeSurface(Surface);

        // Unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // DESTRUCTOR

    // Properly destroys this image.
    Image::~Image() {
        // Destroys the pixels array
        if (MyPixels != nullptr) {
            for (int x = 0; x < MyTextureSize.x; ++x) {
                delete[] MyPixels[x];
            }
            delete[] MyPixels;
        }

        // Destroys the texture in OpenGL
        if (MyTextureID != 0) {
            glDeleteTextures(1, &MyTextureID);
            MyTextureID = 0;
        }

        --TotalImages;

        // Close SDL_image if there are no more images remaining
        if (TotalImages == 0) {
            IMG_Quit();
        }
    }

    // GETTERS

    // Returns the path to this image's texture.
    std::string Image::Path() const {
        return ImagePath;
    }

    // Returns the size in pixels of this image's texture.
    glm::ivec2 Image::TextureSize() const {
        return MyTextureSize;
    }

    // Returns the ID of this texture in OpenGL.
    GLuint Image::TextureID() const {
        return MyTextureID;
    }

    // Returns a 2D array of each of this image's pixel colors or nullptr if there are none.
    // The color at [0][0] will always be the top left.
    SDL_Color const *const*Image::Pixels() const {
        return MyPixels;
    }

    // Returns the total number of images currently active.
    int Image::Total() {
        return TotalImages;
    }

    // STATIC VARIABLE INITIALIZATION

    // The current number of images open.
    int Image::TotalImages = 0;
}
