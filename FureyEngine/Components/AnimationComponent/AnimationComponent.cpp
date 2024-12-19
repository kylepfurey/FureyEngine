// .cpp
// Animation Component Class Script
// by Kyle Furey

#include "AnimationComponent.h"

namespace FureyEngine {
    // ANIMATION SEQUENCE

    // Constructs a new animation sequence.
    AnimationComponent::AnimationSequence::AnimationSequence(std::string Name, FureyEngine::Animation *Animation,
                                                             const int &Framerate, const int &CurrentFrame)
        : Name(std::move(Name)), Animation(Animation),
          Framerate(Framerate), CurrentFrame(CurrentFrame) {
    }

    // CONSTRUCTORS

    // Constructs a new animation component.
    AnimationComponent::AnimationComponent(std::string CurrentAnimation, Animation *Animation, const int &Framerate,
                                           const Reference<FureyEngine::RenderComponent> &RenderComponent,
                                           const int &Speed,
                                           const bool &Loop, const bool &Paused, const int &FrameSkip,
                                           const bool &TickLate)
        : Component("Animation", true),
          CurrentAnimation(std::move(CurrentAnimation)), RenderComponent(RenderComponent),
          Speed(Speed), Loop(Loop), Paused(Paused), FrameSkip(FrameSkip), TickLate(TickLate) {
        Animations.try_emplace(this->CurrentAnimation, this->CurrentAnimation, Animation, Framerate);
    }

    // EVENTS

    // Automatically called before each component has already called Start().
    void AnimationComponent::Attach() {
        // Calls the base class's function
        Component::Attach();

        // Bind set active to the actor's set active function
        GetActor()->OnSetActive.Bind(std::bind(AnimationComponent::SetActive, this, std::placeholders::_1));

        // Finds or attaches a render component if it does not exist
        if (!((RenderComponent = GetActor()->GetComponent<FureyEngine::RenderComponent>()))) {
            RenderComponent = GetActor()->AttachComponent<FureyEngine::RenderComponent>(nullptr);
        }

        // Sets the animation delay
        Delay = 1.0 / Animations.at(CurrentAnimation).Framerate;

        // Sets the render component's texture
        RenderComponent->SetImage(Animations.at(CurrentAnimation).Animation != nullptr
                                      ? Animations.at(CurrentAnimation).Animation->Frame(0)
                                      : nullptr);
    }

    // Automatically called each world tick.
    void AnimationComponent::Tick(const double &DeltaTime) {
        // Calls the base class's function
        Component::Tick(DeltaTime);

        // Update the animation
        if (!TickLate) {
            if (Paused) {
                return;
            }

            // Pause the animation if we are not looping and have reached the end
            if (!Loop && Animations.at(CurrentAnimation).Animation != nullptr &&
                Animations.at(CurrentAnimation).CurrentFrame ==
                Animations.at(CurrentAnimation).Animation->Count() - 1) {
                Delay = 1.0 / Animations.at(CurrentAnimation).Framerate;
                return;
            }

            Delay -= DeltaTime * Speed;

            while (Delay <= 0) {
                // Advance the animation frame
                Animations.at(CurrentAnimation).CurrentFrame =
                        Animations.at(CurrentAnimation).Animation != nullptr
                            ? (Animations.at(CurrentAnimation).CurrentFrame + FrameSkip) %
                              Animations.at(CurrentAnimation).Animation->Count()
                            : 0;

                // Update the render component's image
                if (RenderComponent) {
                    RenderComponent->SetImage(
                        Animations.at(CurrentAnimation).Animation != nullptr
                            ? Animations.at(CurrentAnimation).Animation->Frame(
                                Animations.at(CurrentAnimation).CurrentFrame)
                            : nullptr);
                }

                // Resets the delay
                Delay += 1.0 / Animations.at(CurrentAnimation).Framerate;
            }
        }
    }

    // Automatically called after each component has already called Tick().
    void AnimationComponent::LateTick(const double &DeltaTime) {
        // Calls the base class's function
        Component::LateTick(DeltaTime);

        // Update the animation
        if (TickLate) {
            if (Paused) {
                return;
            }

            // Pause the animation if we are not looping and have reached the end
            if (!Loop && Animations.at(CurrentAnimation).Animation != nullptr &&
                Animations.at(CurrentAnimation).CurrentFrame ==
                Animations.at(CurrentAnimation).Animation->Count() - 1) {
                Delay = 1.0 / Animations.at(CurrentAnimation).Framerate;
                return;
            }

            Delay -= DeltaTime * Speed;

            while (Delay <= 0) {
                // Advance the animation frame
                Animations.at(CurrentAnimation).CurrentFrame =
                        Animations.at(CurrentAnimation).Animation != nullptr
                            ? (Animations.at(CurrentAnimation).CurrentFrame + FrameSkip) %
                              Animations.at(CurrentAnimation).Animation->Count()
                            : 0;

                // Update the render component's image
                if (RenderComponent) {
                    RenderComponent->SetImage(
                        Animations.at(CurrentAnimation).Animation != nullptr
                            ? Animations.at(CurrentAnimation).Animation->Frame(
                                Animations.at(CurrentAnimation).CurrentFrame)
                            : nullptr);
                }

                // Resets the delay
                Delay += 1.0 / Animations.at(CurrentAnimation).Framerate;
            }
        }
    }

    // Automatically called after this component is removed.
    void AnimationComponent::Remove() {
        // Calls the base class's function
        Component::Remove();

        // Unbind set active from the actor's set active function
        if (BeginTick) {
            GetActor()->OnSetActive.Unbind(std::bind(AnimationComponent::SetActive, this, std::placeholders::_1));
        }
    }

    // GETTERS

    // Returns a pointer to the current animation.
    AnimationComponent::AnimationSequence *AnimationComponent::GetCurrentAnimation() {
        return &Animations.at(CurrentAnimation);
    }

    // Returns a pointer to the given animation or nullptr if it does not exist.
    AnimationComponent::AnimationSequence *AnimationComponent::GetAnimation(const std::string &AnimationName) {
        return Animations.count(AnimationName) ? &Animations.at(AnimationName) : nullptr;
    }

    // Returns a pointer to the given animation or nullptr if it does not exist.
    AnimationComponent::AnimationSequence *AnimationComponent::operator[](const std::string &AnimationName) {
        return Animations.count(AnimationName) ? &Animations.at(AnimationName) : nullptr;
    }

    // Returns a map of each animation name to a pointer to that animation sequence.
    std::map<std::string, AnimationComponent::AnimationSequence *> AnimationComponent::GetAllAnimations() {
        std::map<std::string, AnimationSequence *> Animations;
        for (auto &[Key, Value]: this->Animations) {
            Animations.at(Key) = &Value;
        }
        return Animations;
    }

    // Returns whether the given animation exists in this component by name.
    bool AnimationComponent::HasAnimation(const std::string &AnimationName) const {
        return Animations.count(AnimationName);
    }

    // Returns the number of animations in this component.
    int AnimationComponent::TotalAnimations() const {
        return static_cast<int>(Animations.size());
    }


    // SETTERS

    // Sets the current animation by name and its current frame.
    // An index of less than zero uses the current frame index.
    // Returns whether the animation was successfully set.
    bool AnimationComponent::SetCurrentAnimation(const std::string &CurrentAnimation, const int &FrameIndex) {
        if (Animations.count(CurrentAnimation)) {
            this->CurrentAnimation = CurrentAnimation;

            // Set the animation frame
            if (FrameIndex >= 0) {
                Animations.at(CurrentAnimation).CurrentFrame =
                        Animations.at(CurrentAnimation).Animation != nullptr
                            ? FrameIndex % Animations.at(CurrentAnimation).Animation->Count()
                            : 0;
            }

            // Update the render component's image
            if (RenderComponent) {
                RenderComponent->SetImage(
                    Animations.at(CurrentAnimation).Animation != nullptr
                        ? Animations.at(CurrentAnimation).Animation->Frame(Animations.at(CurrentAnimation).CurrentFrame)
                        : nullptr);
            }

            // Reset the delay
            Delay = 1.0 / Animations.at(CurrentAnimation).Framerate;

            return true;
        }

        return false;
    }

    // Adds or overwrites a new animation to this component.
    // Returns whether an animation was overwritten.
    bool AnimationComponent::AddAnimation(const std::string &AnimationName, Animation *Animation,
                                          const int &Framerate) {
        if (!Animations.count(AnimationName)) {
            Animations.try_emplace(AnimationName, AnimationName, Animation, Framerate);
            return false;
        }

        Animations.erase(AnimationName);
        Animations.try_emplace(AnimationName, AnimationName, Animation, Framerate);
        return true;
    }

    // Remove an animation by name from this component.
    // Returns whether the animation was successfully removed.
    bool AnimationComponent::RemoveAnimation(const std::string &AnimationName) {
        if (Animations.count(AnimationName)) {
            Animations.erase(AnimationName);
            return true;
        }

        return false;
    }
}
