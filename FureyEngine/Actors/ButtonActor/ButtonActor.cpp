// .cpp
// Clickable Button Actor Class Script
// by Kyle Furey

#include "ButtonActor.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Constructs a new button with a name.
    ButtonActor::ButtonActor(const std::string &Name, const bool &Active)
        : Actor(Name, Active) {
    }

    // EVENTS

    // Automatically called each world tick.
    void ButtonActor::Tick(const double &DeltaTime) {
        // Calls the base class's function
        Actor::Tick(DeltaTime);

        // Update the state of the button
        if (!TickLate) {
            const bool WasHovered = Hovered;
            Hovered = Clickable && Camera && BrushComponent &&
                      BrushComponent->IsWithinBounds(Camera->GetMouseWorldPosition());

            if (Hovered) {
                if (!WasHovered) {
                    OnHover();
                }

                State = Input::GetMouseButtonState(LEFT_MOUSE_BUTTON);

                // Click the button
                if (State == (ClickOnRelease ? ButtonState::RELEASED : ButtonState::DOWN)) {
                    if (Toggle) {
                        Clicked = !Clicked;

                        if (Clicked) {
                            OnClick();
                        } else {
                            OnUnclick();
                        }
                    } else {
                        Clicked = true;

                        OnClick();
                    }
                }

                // Unclick the button
                if (!Toggle && Clicked && State == ButtonState::RELEASED) {
                    Clicked = false;
                    OnUnclick();
                }

                if (RenderComponent) {
                    switch (State) {
                        case ButtonState::UP:
                            RenderComponent->SetImage(HoveredImage);
                            break;
                        case ButtonState::DOWN:
                            RenderComponent->SetImage(ClickedImage);
                            break;
                        case ButtonState::HELD:
                            RenderComponent->SetImage(ClickedImage);
                            break;
                        case ButtonState::RELEASED:
                            RenderComponent->SetImage(HoveredImage);
                            break;
                    }

                    if (TextComponent) {
                        TextComponent->RenderLast();
                    }
                }
            } else {
                State = ButtonState::UP;

                // Unclick the button
                if (!Toggle && Clicked) {
                    Clicked = false;
                    OnUnclick();
                }

                if (RenderComponent) {
                    if (!Toggle || !Clicked) {
                        RenderComponent->SetImage(UnclickedImage);
                    } else {
                        RenderComponent->SetImage(ClickedImage);
                    }

                    if (TextComponent) {
                        TextComponent->RenderLast();
                    }
                }
            }
        }
    }

    // Automatically called after each actor has already called Tick().
    void ButtonActor::LateTick(const double &DeltaTime) {
        // Calls the base class's function
        Actor::LateTick(DeltaTime);

        // Update the state of the button
        if (TickLate) {
            const bool WasHovered = Hovered;
            Hovered = Clickable && Camera && BrushComponent &&
                      BrushComponent->IsWithinBounds(Camera->GetMouseWorldPosition());

            if (Hovered) {
                if (!WasHovered) {
                    OnHover();
                }

                State = Input::GetMouseButtonState(LEFT_MOUSE_BUTTON);

                // Click the button
                if (State == (ClickOnRelease ? ButtonState::RELEASED : ButtonState::DOWN)) {
                    if (Toggle) {
                        Clicked = !Clicked;

                        if (Clicked) {
                            OnClick();
                        } else {
                            OnUnclick();
                        }
                    } else {
                        Clicked = true;

                        OnClick();
                    }
                }

                // Unclick the button
                if (!Toggle && Clicked && State == ButtonState::RELEASED) {
                    Clicked = false;
                    OnUnclick();
                }

                if (RenderComponent) {
                    switch (State) {
                        case ButtonState::UP:
                            RenderComponent->SetImage(HoveredImage);
                            break;
                        case ButtonState::DOWN:
                            RenderComponent->SetImage(ClickedImage);
                            break;
                        case ButtonState::HELD:
                            RenderComponent->SetImage(ClickedImage);
                            break;
                        case ButtonState::RELEASED:
                            RenderComponent->SetImage(HoveredImage);
                            break;
                    }

                    if (TextComponent) {
                        TextComponent->RenderLast();
                    }
                }
            } else {
                State = ButtonState::UP;

                // Unclick the button
                if (!Toggle && Clicked) {
                    Clicked = false;
                    OnUnclick();
                }

                if (RenderComponent) {
                    if (!Toggle || !Clicked) {
                        RenderComponent->SetImage(UnclickedImage);
                    } else {
                        RenderComponent->SetImage(ClickedImage);
                    }

                    if (TextComponent) {
                        TextComponent->RenderLast();
                    }
                }
            }
        }
    }

    // ACTIVITY

    // Sets whether this actor is active.
    // This can be overridden to control custom functionality.
    void ButtonActor::SetActive(const bool &Active) {
        // Calls the base class's function
        Actor::SetActive(Active);

        if (!Active) {
            State = ButtonState::UP;

            // Unclick the button
            if (!Toggle && Clicked) {
                Clicked = false;
                OnUnclick();
            }

            if (RenderComponent) {
                RenderComponent->SetImage(UnclickedImage);

                if (TextComponent) {
                    TextComponent->RenderLast();
                }
            }
        }
    }

    // BUTTON

    // Returns whether this button has been initialized yet.
    bool ButtonActor::IsInitialized() const {
        return Initialized;
    }

    // Initializes this button with the given parameters.
    Reference<ButtonActor> ButtonActor::Initialize(const Reference<CameraComponent> &Camera,
                                                   Image *UnclickedImage,
                                                   Image *HoveredImage,
                                                   Image *ClickedImage,
                                                   const Font *Font,
                                                   const std::string &Text,
                                                   const int &Size,
                                                   const SDL_Color &Color) {
        if (!Initialized) {
            Initialized = true;
            this->Camera = Camera;
            this->UnclickedImage = UnclickedImage;
            this->HoveredImage = HoveredImage == nullptr ? UnclickedImage : HoveredImage;
            this->ClickedImage = ClickedImage == nullptr ? UnclickedImage : ClickedImage;
            RenderComponent = AttachComponent<FureyEngine::RenderComponent>(UnclickedImage);
            BrushComponent = AttachComponent<FureyEngine::BrushComponent>(RenderComponent, true);
            TextComponent = AttachComponent<FureyEngine::TextComponent>(Font, Text, Size, Color);
        }
        return {this};
    }

    // Returns the current state of this button.
    ButtonState ButtonActor::GetButtonState() const {
        return State;
    }

    // Returns whether this button is currently hovered over.
    bool ButtonActor::IsHovered() const {
        return Hovered;
    }

    // Returns whether this button is currently clicked down.
    bool ButtonActor::IsClicked() const {
        return Clicked;
    }

    // Clicks the button and calls its events.
    // Returns whether the button is now clicked.
    bool ButtonActor::Click() {
        if (!Clickable) {
            return false;
        }

        State = ButtonState::DOWN;

        if (Toggle) {
            Clicked = !Clicked;

            if (Clicked) {
                OnClick();
            } else {
                OnUnclick();
            }
        } else {
            Clicked = true;

            OnClick();
        }

        if (RenderComponent) {
            RenderComponent->SetImage(ClickedImage);

            if (TextComponent) {
                TextComponent->RenderLast();
            }
        }

        return Clicked;
    }
}
