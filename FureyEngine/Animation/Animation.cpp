// .cpp
// Image Animation Sequence Script
// by Kyle Furey

#include "Animation.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Creates a new animation with the given frame count from the given directory of images.
    // The index of each frame is appended to the given directory, so each frame's name must match.
    // Example: std::string Directory = ROOT"Animations/MyAnimation" = MyAnimation0, MyAnimation1, ...
    Animation::Animation(std::string Directory, const int &FrameCount, const std::string &Extension)
        : AnimationDirectory(std::move(Directory)), FileExtension(Extension) {
        ++TotalAnimations;

        // Creates an image for each frame in the animation sequence
        for (int i = 0; i < FrameCount; ++i) {
            Frames.Add(AnimationDirectory + std::to_string(i) + Extension);
        }
    }

    // DESTRUCTOR

    // Properly destroys this animation sequence's images.
    Animation::~Animation() {
        --TotalAnimations;
    }

    // GETTERS

    // Returns the directory of this animation sequence's frames.
    std::string Animation::Directory() const {
        return AnimationDirectory;
    }

    // Returns each frame's file extension.
    std::string Animation::Extension() const {
        return FileExtension;
    }

    // Returns a vector of a pointer to each frame in the animation sequence.
    std::vector<Image *> Animation::Sequence() const {
        std::vector<Image *> Sequence;
        for (int i = 0; i < Frames.Count(); ++i) {
            Sequence.push_back(&Frames[i]);
        }
        return Sequence;
    }

    // Returns a pointer to the given frame in the animation sequence.
    Image *Animation::Frame(const int &Index) const {
        return &Frames[Index % Frames.Count()];
    }

    // Returns a pointer to the given frame in the animation sequence.
    const Image *Animation::operator[](const int &Index) const {
        return &Frames[Index % Frames.Count()];
    }

    // Returns the total number of frames in this animation sequence.
    int Animation::Count() const {
        return Frames.Count();
    }

    // Returns the total number of animation sequences currently active.
    int Animation::Total() {
        return TotalAnimations;
    }

    // STATIC VARIABLE INITIALIZATION

    // The current number of animations open.
    int Animation::TotalAnimations = 0;
}
