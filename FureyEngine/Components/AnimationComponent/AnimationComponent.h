// .h
// Animation Component Class Script
// by Kyle Furey

#pragma once
#include "../RenderComponent/RenderComponent.h"

#define DEFAULT_FRAMERATE 30

namespace FureyEngine {
    /** A component used to combine images into sequences to be animated on a render component. */
    class AnimationComponent : public Component {
    public:
        // ANIMATION SEQUENCE

        /** Represents an animation. */
        struct AnimationSequence {
            // SEQUENCE

            /** The name of this animation sequence. */
            const std::string Name;

            /** A pointer to the animation sequence to play. */
            Animation *Animation = nullptr;

            /** The number of frames per second to iterate through in the animation sequence. */
            int Framerate = DEFAULT_FRAMERATE;

            /** The index of the current frame of the animation sequence. */
            int CurrentFrame = 0;

            // CONSTRUCTORS

            /** Constructs a new animation sequence. */
            AnimationSequence(std::string Name, FureyEngine::Animation *Animation,
                              const int &Framerate = DEFAULT_FRAMERATE, const int &CurrentFrame = 0);
        };

    protected:
        // ANIMATION

        /** The current animation sequence of this component. */
        std::string CurrentAnimation = "main";

        /** Each animation sequence in this component by name. */
        std::map<std::string, AnimationSequence> Animations;

        /** The remaining time in seconds before advancing a frame in the animation. */
        double Delay = 0;

    public:
        // ANIMATION

        /**
         * A reference to the render component to play animations for.<br/>
         * This is automatically attached if one isn't specified or found.
         */
        Reference<RenderComponent> RenderComponent = nullref;

        /** A multiplier to apply to each animation's speed. */
        double Speed = 1;

        /** Whether to loop the animation. */
        bool Loop = true;

        /** Whether the animation is currently paused. */
        bool Paused = false;

        /** The number of frames to advance in each animation. */
        int FrameSkip = 1;

        /** Whether to use LateTick() instead of Tick(). */
        bool TickLate = true;

        // CONSTRUCTORS

        /** Constructs a new animation component. */
        explicit AnimationComponent(std::string CurrentAnimation,
                                    Animation *Animation,
                                    const int &Framerate = DEFAULT_FRAMERATE,
                                    const Reference<FureyEngine::RenderComponent> &RenderComponent = nullref,
                                    const int &Speed = 1,
                                    const bool &Loop = true, const bool &Paused = false, const int &FrameSkip = 1,
                                    const bool &TickLate = true);

        // EVENTS

        /** Automatically called before each component has already called Start(). */
        void Attach() override;

        /** Automatically called each world tick. */
        void Tick(const double &DeltaTime) override;

        /** Automatically called after each component has already called Tick(). */
        void LateTick(const double &DeltaTime) override;

        /** Automatically called after this component is removed. */
        void Remove() override;

        // GETTERS

        /** Returns a pointer to the current animation. */
        [[nodiscard]] virtual AnimationSequence *GetCurrentAnimation();

        /** Returns a pointer to the given animation or nullptr if it does not exist. */
        [[nodiscard]] virtual AnimationSequence *GetAnimation(const std::string &AnimationName);

        /** Returns a pointer to the given animation or nullptr if it does not exist. */
        [[nodiscard]] virtual AnimationSequence *operator[](const std::string &AnimationName);

        /** Returns a map of each animation name to a pointer to that animation sequence. */
        [[nodiscard]] virtual std::map<std::string, AnimationSequence *> GetAllAnimations();

        /** Returns whether the given animation exists in this component by name. */
        [[nodiscard]] virtual bool HasAnimation(const std::string &AnimationName) const;

        /** Returns the number of animations in this component. */
        [[nodiscard]] virtual int TotalAnimations() const;

        // SETTERS

        /**
         * Sets the current animation by name and its current frame.<br/>
         * An index of less than zero uses the current frame index.<br/>
         * Returns whether the animation was successfully set.
         */
        virtual bool SetCurrentAnimation(const std::string &CurrentAnimation, const int &FrameIndex = -1);

        /**
         * Adds or overwrites a new animation to this component.<br/>
         * Returns whether an animation was overwritten.
         */
        virtual bool AddAnimation(const std::string &AnimationName,
                                  Animation *Animation,
                                  const int &Framerate = DEFAULT_FRAMERATE);

        /**
         * Remove an animation by name from this component.<br/>
         * Returns whether the animation was successfully removed.
         */
        virtual bool RemoveAnimation(const std::string &AnimationName);
    };
}
