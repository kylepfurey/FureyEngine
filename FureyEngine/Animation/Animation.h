// .h
// Image Animation Sequence Script
// by Kyle Furey

#pragma once
#include "../Image/Image.h"
#include "../PointerArray/PointerArray.h"

namespace FureyEngine {
    /** Creates and stores a static collection of images that can be played in a sequence to create an animation. */
    class Animation final {
        // DIRECTORY

        /** The folder where the animation sequence is stored. */
        const std::string AnimationDirectory;

        // EXTENSION

        /** The file extension of each frame. */
        const std::string FileExtension;

        // SEQUENCE

        /** An array of images that make up this animation sequence. */
        PointerArray<Image> Frames;

        // TOTAL IMAGES

        /** The current number of animation sequences open. */
        static int TotalAnimations;

    public:
        // CONSTRUCTORS

        /**
         * Creates a new animation with the given frame count from the given directory of images.<br/>
         * The index of each frame is appended to the given directory, so each frame's name must match.<br/>
         * Example: std::string Directory = ROOT"Animations/MyAnimation" = MyAnimation0, MyAnimation1, ...
         */
        Animation(std::string Directory, const int &FrameCount, const std::string &Extension = ".png");

        /** Delete copy constructor. */
        Animation(const Animation &Copied) = delete;

        /** Delete move constructor. */
        Animation(Animation &&Moved) = delete;

        // ASSIGNMENT OPERATORS

        /** Delete copy constructor. */
        Animation &operator=(const Animation &Copied) = delete;

        /** Delete move constructor. */
        Animation &operator=(Animation &&Moved) = delete;

        // DESTRUCTOR

        /** Properly destroys this animation sequence's images. */
        ~Animation();

        // GETTERS

        /** Returns the directory of this animation sequence's frames. */
        [[nodiscard]] std::string Directory() const;

        /** Returns each frame's file extension. */
        [[nodiscard]] std::string Extension() const;

        /** Returns a vector of a pointer to each frame in the animation sequence. */
        [[nodiscard]] std::vector<Image *> Sequence() const;

        /** Returns a pointer to the given frame in the animation sequence. */
        [[nodiscard]] Image *Frame(const int &Index) const;

        /** Returns a pointer to the given frame in the animation sequence. */
        [[nodiscard]] const Image *operator[](const int &Index) const;

        /** Returns the total number of frames in this animation sequence. */
        [[nodiscard]] int Count() const;

        /** Returns the total number of animation sequences currently active. */
        [[nodiscard]] static int Total();
    };
}
