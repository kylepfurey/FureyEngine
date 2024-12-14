// .cpp
// Movement Component Class Script
// by Kyle Furey

#include "MovementComponent.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Constructs a new movement component.
    MovementComponent::MovementComponent(const glm::vec2 &Speed, const glm::vec2 &Acceleration,
                                         const int &ControllerIndex,
                                         const Reference<FureyEngine::BrushComponent> &BrushComponent,
                                         const bool &RelativeToRotation, const float &SpeedModifier,
                                         const bool &TickLate)
        : Component("Movement", true),
          Speed(Speed), Acceleration(Acceleration), Deceleration(Acceleration),
          UseKeyboard(ControllerIndex < 0), ControllerIndex(ControllerIndex), BrushComponent(BrushComponent),
          RelativeToRotation(RelativeToRotation), SpeedModifier(SpeedModifier), TickLate(TickLate) {
    }

    // EVENTS

    // Automatically called before each component has already called Start().
    void MovementComponent::Attach() {
        // Calls the base class's function
        Component::Attach();

        // Bind set active to the actor's set active function
        GetActor()->OnSetActive.Bind(std::bind(MovementComponent::SetActive, this, std::placeholders::_1));

        if (!BrushComponent) {
            BrushComponent = GetActor()->GetComponent<FureyEngine::BrushComponent>();
        }
    }

    // Automatically called each world tick.
    void MovementComponent::Tick(const double &DeltaTime) {
        // Calls the base class's function
        Component::Tick(DeltaTime);

        // Move the actor
        if (!TickLate) {
            glm::vec2 Input = {0, 0};

            // Keyboard input
            if (UseKeyboard) {
#if MOVE_WITH_WASD
                if (Input::IsKeyDown(SDLK_a) ||
                    Input::IsKeyHeld(SDLK_a)) {
                    Input.x = -1;
                } else if (Input::IsKeyDown(SDLK_d) ||
                           Input::IsKeyHeld(SDLK_d)) {
                    Input.x = 1;
                }

                if (Input::IsKeyDown(SDLK_w) ||
                    Input::IsKeyHeld(SDLK_w)) {
                    Input.y = 1;
                } else if (Input::IsKeyDown(SDLK_s) ||
                           Input::IsKeyHeld(SDLK_s)) {
                    Input.y = -1;
                }
#endif

#if MOVE_WITH_ARROW_KEYS
                if (Input.x == 0 && Input.y == 0) {
                    if (Input::IsKeyDown(SDLK_LEFT) ||
                        Input::IsKeyHeld(SDLK_LEFT)) {
                        Input.x = -1;
                    } else if (Input::IsKeyDown(SDLK_RIGHT) ||
                               Input::IsKeyHeld(SDLK_RIGHT)) {
                        Input.x = 1;
                    }

                    if (Input::IsKeyDown(SDLK_UP) ||
                        Input::IsKeyHeld(SDLK_UP)) {
                        Input.y = 1;
                    } else if (Input::IsKeyDown(SDLK_DOWN) ||
                               Input::IsKeyHeld(SDLK_DOWN)) {
                        Input.y = -1;
                    }
                }
#endif
            }

            // Controller input
            if (Input.x == 0 && Input.y == 0 && Engine::Controllers.InRange(ControllerIndex)) {
                const auto *Controller = &Engine::Controllers[ControllerIndex];
#if MOVE_WITH_LEFT_STICK
                Input = Controller->GetLeftStick();
#endif

#if MOVE_WITH_D_PAD
                if (Input.x == 0 && Input.y == 0) {
                    if (Controller->IsButtonDown(SDL_CONTROLLER_BUTTON_DPAD_LEFT) ||
                        Controller->IsButtonHeld(SDL_CONTROLLER_BUTTON_DPAD_LEFT)) {
                        Input.x = -1;
                    } else if (Controller->IsButtonDown(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) ||
                               Controller->IsButtonHeld(SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) {
                        Input.x = 1;
                    }

                    if (Controller->IsButtonDown(SDL_CONTROLLER_BUTTON_DPAD_UP) ||
                        Controller->IsButtonHeld(SDL_CONTROLLER_BUTTON_DPAD_UP)) {
                        Input.y = 1;
                    } else if (Controller->IsButtonDown(SDL_CONTROLLER_BUTTON_DPAD_DOWN) ||
                               Controller->IsButtonHeld(SDL_CONTROLLER_BUTTON_DPAD_DOWN)) {
                        Input.y = -1;
                    }
                }
#endif
            }

            if (Input.x != 0 || Input.y != 0) {
                // Normalize our input
                const auto Magnitude = static_cast<float>(Math::Magnitude(Input));
                Input = Math::Normalize(Input);
                Input.x *= Magnitude;
                Input.y *= Magnitude;

                // Rotate our input if we are relative to the actor's rotation
                if (RelativeToRotation) {
                    Input = Math::Rotate(Input, GetActor()->Transform.Rotation);
                }

                // Increase our velocity
                Velocity.x = Math::Lerp(Velocity.x, Speed.x * Input.x,
                                        Acceleration.x >= 0
                                            ? Acceleration.x * static_cast<float>(DeltaTime * SpeedModifier)
                                            : 1);
                Velocity.y = Math::Lerp(Velocity.y, Speed.y * Input.y,
                                        Acceleration.y >= 0
                                            ? Acceleration.y * static_cast<float>(DeltaTime * SpeedModifier)
                                            : 1);
            } else {
                // Decelerate our velocity
                Velocity.x = Math::Lerp(Velocity.x, 0.0f,
                                        Deceleration.x >= 0
                                            ? Deceleration.x * static_cast<float>(DeltaTime * SpeedModifier)
                                            : 1);
                Velocity.y = Math::Lerp(Velocity.y, 0.0f,
                                        Deceleration.y >= 0
                                            ? Deceleration.y * static_cast<float>(DeltaTime * SpeedModifier)
                                            : 1);
            }

            // Move our current position
            GetActor()->Transform.Position.x += Velocity.x * static_cast<float>(DeltaTime * SpeedModifier);
            GetActor()->Transform.Position.y += Velocity.y * static_cast<float>(DeltaTime * SpeedModifier);

            // Move back if our brush collides with an object
            if (BrushComponent && BrushComponent->IsColliding()) {
                GetActor()->Transform.Position.x -= Velocity.x * static_cast<float>(DeltaTime * SpeedModifier);
                GetActor()->Transform.Position.y -= Velocity.y * static_cast<float>(DeltaTime * SpeedModifier);
                Velocity.x = 0;
                Velocity.y = 0;
            }
        }
    }

    // Automatically called after each component has already called Tick().
    void MovementComponent::LateTick(const double &DeltaTime) {
        // Calls the base class's function
        Component::LateTick(DeltaTime);

        // Move the actor
        if (TickLate) {
            glm::vec2 Input = {0, 0};

            // Keyboard input
            if (UseKeyboard) {
#if MOVE_WITH_WASD
                if (Input::IsKeyDown(SDLK_a) ||
                    Input::IsKeyHeld(SDLK_a)) {
                    Input.x = -1;
                } else if (Input::IsKeyDown(SDLK_d) ||
                           Input::IsKeyHeld(SDLK_d)) {
                    Input.x = 1;
                }

                if (Input::IsKeyDown(SDLK_w) ||
                    Input::IsKeyHeld(SDLK_w)) {
                    Input.y = 1;
                } else if (Input::IsKeyDown(SDLK_s) ||
                           Input::IsKeyHeld(SDLK_s)) {
                    Input.y = -1;
                }
#endif

#if MOVE_WITH_ARROW_KEYS
                if (Input.x == 0 && Input.y == 0) {
                    if (Input::IsKeyDown(SDLK_LEFT) ||
                        Input::IsKeyHeld(SDLK_LEFT)) {
                        Input.x = -1;
                    } else if (Input::IsKeyDown(SDLK_RIGHT) ||
                               Input::IsKeyHeld(SDLK_RIGHT)) {
                        Input.x = 1;
                    }

                    if (Input::IsKeyDown(SDLK_UP) ||
                        Input::IsKeyHeld(SDLK_UP)) {
                        Input.y = 1;
                    } else if (Input::IsKeyDown(SDLK_DOWN) ||
                               Input::IsKeyHeld(SDLK_DOWN)) {
                        Input.y = -1;
                    }
                }
#endif
            }

            // Controller input
            if (Input.x == 0 && Input.y == 0 && Engine::Controllers.InRange(ControllerIndex)) {
                const auto *Controller = &Engine::Controllers[ControllerIndex];
#if MOVE_WITH_LEFT_STICK
                Input = Controller->GetLeftStick();
#endif

#if MOVE_WITH_D_PAD
                if (Input.x == 0 && Input.y == 0) {
                    if (Controller->IsButtonDown(SDL_CONTROLLER_BUTTON_DPAD_LEFT) ||
                        Controller->IsButtonHeld(SDL_CONTROLLER_BUTTON_DPAD_LEFT)) {
                        Input.x = -1;
                    } else if (Controller->IsButtonDown(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) ||
                               Controller->IsButtonHeld(SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) {
                        Input.x = 1;
                    }

                    if (Controller->IsButtonDown(SDL_CONTROLLER_BUTTON_DPAD_UP) ||
                        Controller->IsButtonHeld(SDL_CONTROLLER_BUTTON_DPAD_UP)) {
                        Input.y = 1;
                    } else if (Controller->IsButtonDown(SDL_CONTROLLER_BUTTON_DPAD_DOWN) ||
                               Controller->IsButtonHeld(SDL_CONTROLLER_BUTTON_DPAD_DOWN)) {
                        Input.y = -1;
                    }
                }
#endif
            }

            if (Input.x != 0 || Input.y != 0) {
                // Normalize our input
                const auto Magnitude = static_cast<float>(Math::Magnitude(Input));
                Input = Math::Normalize(Input);
                Input.x *= Magnitude;
                Input.y *= Magnitude;

                // Rotate our input if we are relative to the actor's rotation
                if (RelativeToRotation) {
                    Input = Math::Rotate(Input, GetActor()->Transform.Rotation);
                }

                // Increase our velocity
                Velocity.x = Math::Lerp(Velocity.x, Speed.x * Input.x,
                                        Acceleration.x >= 0
                                            ? Acceleration.x * static_cast<float>(DeltaTime * SpeedModifier)
                                            : 1);
                Velocity.y = Math::Lerp(Velocity.y, Speed.y * Input.y,
                                        Acceleration.y >= 0
                                            ? Acceleration.y * static_cast<float>(DeltaTime * SpeedModifier)
                                            : 1);
            } else {
                // Decelerate our velocity
                Velocity.x = Math::Lerp(Velocity.x, 0.0f,
                                        Deceleration.x >= 0
                                            ? Deceleration.x * static_cast<float>(DeltaTime * SpeedModifier)
                                            : 1);
                Velocity.y = Math::Lerp(Velocity.y, 0.0f,
                                        Deceleration.y >= 0
                                            ? Deceleration.y * static_cast<float>(DeltaTime * SpeedModifier)
                                            : 1);
            }

            // Move our current position
            GetActor()->Transform.Position.x += Velocity.x * static_cast<float>(DeltaTime * SpeedModifier);
            GetActor()->Transform.Position.y += Velocity.y * static_cast<float>(DeltaTime * SpeedModifier);

            // Move back if our brush collides with an object
            if (BrushComponent && BrushComponent->IsColliding()) {
                GetActor()->Transform.Position.x -= Velocity.x * static_cast<float>(DeltaTime * SpeedModifier);
                GetActor()->Transform.Position.y -= Velocity.y * static_cast<float>(DeltaTime * SpeedModifier);
                Velocity.x = 0;
                Velocity.y = 0;
            }
        }
    }

    // Automatically called after this component is removed.
    void MovementComponent::Remove() {
        // Calls the base class's function
        Component::Remove();

        // Unbind set active from the actor's set active function
        if (BeginTick) {
            GetActor()->OnSetActive.Unbind(std::bind(MovementComponent::SetActive, this, std::placeholders::_1));
        }
    }
}
