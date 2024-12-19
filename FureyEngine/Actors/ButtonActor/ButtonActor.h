// .h
// Clickable Button Actor Class Script
// by Kyle Furey

#pragma once
#include "../../Components/TextComponent/TextComponent.h"
#include "../../Components/CameraComponent/CameraComponent.h"
#include "../../Components/BrushComponent/BrushComponent.h"

namespace FureyEngine {
    /** An actor with a set of components that enable it to be clicked on to trigger events. */
    class ButtonActor : public Actor {
    protected:
        // BUTTON

        /** Whether this button has been initialized yet. */
        bool Initialized = false;

        /** The current state of this button. */
        ButtonState State = ButtonState::UP;

        /** Whether this button is currently . */
        bool Hovered = false;

        /** Whether this button is currently clicked down. */
        bool Clicked = false;

    public:
        // BUTTON

        /**
         * A reference to the render component of this button, used to render and update this button's sprite.<br/>
         * This is automatically attached when Initialize() is called.
         */
        Reference<RenderComponent> RenderComponent = nullref;

        /**
         * A reference to the text component of this button, used to render optional text on this button.<br/>
         * This is automatically attached when Initialize() is called.
         */
        Reference<TextComponent> TextComponent = nullref;

        /** A reference to the brush component of this button, used for mouse and world collision. */
        Reference<BrushComponent> BrushComponent = nullref;

        /** A reference to a camera component used to detect mouse input on this button. */
        Reference<CameraComponent> Camera = nullref;

        /** A pointer to the image to use when this button is unclicked. */
        Image *UnclickedImage;

        /** A pointer to the image to use when this button is hovered. */
        Image *HoveredImage;

        /** A pointer to the image to use when this button is clicked. */
        Image *ClickedImage;

        /** An event to call when this button is hovered over. */
        Event OnHover;

        /** An event to call when this button is clicked. */
        Event OnClick;

        /** An event to call when this button is no longer clicked. */
        Event OnUnclick;

        /** Whether this button is currently clickable. */
        bool Clickable = true;

        /** Whether clicking this button will continuously keep it clicked until it is unclicked. */
        bool Toggle = false;

        /** Whether to click the button when its state is released. */
        bool ClickOnRelease = true;

        /** Whether to use LateTick() instead of Tick(). */
        bool TickLate = true;

        // CONSTRUCTORS

        /** Constructs a new button with a name. */
        explicit ButtonActor(const std::string &Name = "Button", const bool &Active = true);

        // EVENTS

        /** Automatically called before each actor has already called Start(). */
        void Spawn() override;

        /** Automatically called each world tick. */
        void Tick(const double &DeltaTime) override;

        /** Automatically called after each actor has already called Tick(). */
        void LateTick(const double &DeltaTime) override;

        /** Automatically called after this actor is destroyed. */
        void Destroy() override;

        // ACTIVITY

        /**
         * Sets whether this actor is active.<br/>
         * This can be overridden to control custom functionality.
         */
        void SetActive(const bool &Active) override;

        // BUTTON

        /** Returns whether this button has been initialized yet. */
        [[nodiscard]] virtual bool IsInitialized() const;

        /** Initializes this button with the given parameters. */
        virtual Reference<ButtonActor> Initialize(const Reference<CameraComponent> &Camera,
                                                  Image *UnclickedImage,
                                                  Image *HoveredImage = nullptr,
                                                  Image *ClickedImage = nullptr,
                                                  const Font *Font = nullptr,
                                                  const std::string &Text = "",
                                                  const int &Size = 32,
                                                  const SDL_Color &Color = {255, 255, 255, 255});

        /** Returns the current state of this button. */
        [[nodiscard]] virtual ButtonState GetButtonState() const;

        /** Returns whether this button is currently hovered over. */
        [[nodiscard]] virtual bool IsHovered() const;

        /** Returns whether this button is currently clicked down. */
        [[nodiscard]] virtual bool IsClicked() const;

        /**
         * Clicks the button and calls its events.<br/>
         * Returns whether the button is now clicked.
         */
        virtual bool Click();
    };
}
