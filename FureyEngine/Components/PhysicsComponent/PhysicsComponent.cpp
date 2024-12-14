// .cpp
// Physics Component Class Script
// by Kyle Furey

#include "PhysicsComponent.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Constructs a new physics component.
    PhysicsComponent::PhysicsComponent(const Reference<FureyEngine::BrushComponent> &BrushComponent,
                                       const glm::vec2 &Gravity, const bool &UseGravity,
                                       const float &Friction, const float &Bounciness, const float &Mass,
                                       const float &SpeedModifier, const bool &TickLate)
        : Component("Physics", true),
          BrushComponent(BrushComponent),
          Gravity(Gravity), UseGravity(UseGravity),
          Friction(Friction), Bounciness(Bounciness), Mass(Mass),
          SpeedModifier(SpeedModifier), TickLate(TickLate) {
    }

    // EVENTS

    // Automatically called before each component has already called Start().
    void PhysicsComponent::Attach() {
        // Calls the base class's function
        Component::Attach();

        // Bind set active to the actor's set active function
        GetActor()->OnSetActive.Bind(std::bind(PhysicsComponent::SetActive, this, std::placeholders::_1));

        if (!BrushComponent) {
            BrushComponent = GetActor()->GetComponent<FureyEngine::BrushComponent>();
        }
    }

    // Automatically called each world tick.
    void PhysicsComponent::Tick(const double &DeltaTime) {
        // Calls the base class's function
        Component::Tick(DeltaTime);

        // Move the actor
        if (!TickLate) {
            // Apply friction
            GetActor()->Transform.Scale.x *= 1.001;
            GetActor()->Transform.Scale.y *= 1.001;

            if (BrushComponent && BrushComponent->IsColliding()) {
                // Move x
                if (Velocity.x >= 0) {
                    Velocity.x -= static_cast<float>(Friction * DeltaTime * SpeedModifier);
                    if (Velocity.x < 0) {
                        Velocity.x = 0;
                    }
                } else {
                    Velocity.x += static_cast<float>(Friction * DeltaTime * SpeedModifier);
                    if (Velocity.x > 0) {
                        Velocity.x = 0;
                    }
                }

                // Move y
                if (Velocity.y >= 0) {
                    Velocity.y -= static_cast<float>(Friction * DeltaTime * SpeedModifier);
                    if (Velocity.y < 0) {
                        Velocity.y = 0;
                    }
                } else {
                    Velocity.y += static_cast<float>(Friction * DeltaTime * SpeedModifier);
                    if (Velocity.y > 0) {
                        Velocity.y = 0;
                    }
                }

                // Move rotation
                if (AngularVelocity >= 0) {
                    AngularVelocity -= static_cast<float>(Friction * DeltaTime * SpeedModifier);
                    if (AngularVelocity < 0) {
                        AngularVelocity = 0;
                    }
                } else {
                    AngularVelocity += static_cast<float>(Friction * DeltaTime * SpeedModifier);
                    if (AngularVelocity > 0) {
                        AngularVelocity = 0;
                    }
                }
            }

            GetActor()->Transform.Scale.x /= 1.001;
            GetActor()->Transform.Scale.y /= 1.001;

            // Apply velocity and check for collisions
            Reference<FureyEngine::BrushComponent> OtherBrush;
            bool Collision = SDL_fabs(Velocity.x) <= SDL_fabs(Gravity.x) * DeltaTime * 2 &&
                             SDL_fabs(Velocity.y) <= SDL_fabs(Gravity.y) * DeltaTime * 2;

            // Move x
            GetActor()->Transform.Position.x += static_cast<float>(Velocity.x) * DeltaTime * SpeedModifier;
            if (BrushComponent && ((OtherBrush = BrushComponent->IsColliding()))) {
                if (!Collision) {
                    Collision = true;
                    OnCollision(this, static_cast<FureyEngine::BrushComponent *>(OtherBrush));
                    if (auto OtherPhysics = OtherBrush->GetActor()->GetComponent<PhysicsComponent>()) {
                        OtherPhysics->OnCollision(static_cast<PhysicsComponent *>(OtherPhysics),
                                                  static_cast<FureyEngine::BrushComponent *>(BrushComponent));
                    }
                }
                GetActor()->Transform.Position.x -= static_cast<float>(Velocity.x) * DeltaTime * SpeedModifier;
                Velocity.x *= -static_cast<float>(SDL_fabs(Bounciness));
            }

            // Move y
            GetActor()->Transform.Position.y += static_cast<float>(Velocity.y) * DeltaTime * SpeedModifier;
            if (BrushComponent && ((OtherBrush = BrushComponent->IsColliding()))) {
                if (!Collision) {
                    Collision = true;
                    OnCollision(this, static_cast<FureyEngine::BrushComponent *>(OtherBrush));
                    if (auto OtherPhysics = OtherBrush->GetActor()->GetComponent<PhysicsComponent>()) {
                        OtherPhysics->OnCollision(static_cast<PhysicsComponent *>(OtherPhysics),
                                                  static_cast<FureyEngine::BrushComponent *>(BrushComponent));
                    }
                }
                GetActor()->Transform.Position.y -= static_cast<float>(Velocity.y) * DeltaTime * SpeedModifier;
                Velocity.y *= -static_cast<float>(SDL_fabs(Bounciness));
            }

            // Move rotation
            GetActor()->Transform.Rotation += static_cast<float>(AngularVelocity) * DeltaTime * SpeedModifier;
            if (BrushComponent && ((OtherBrush = BrushComponent->IsColliding()))) {
                if (!Collision) {
                    Collision = true;
                    OnCollision(this, static_cast<FureyEngine::BrushComponent *>(OtherBrush));
                    if (auto OtherPhysics = OtherBrush->GetActor()->GetComponent<PhysicsComponent>()) {
                        OtherPhysics->OnCollision(static_cast<PhysicsComponent *>(OtherPhysics),
                                                  static_cast<FureyEngine::BrushComponent *>(BrushComponent));
                    }
                }
                GetActor()->Transform.Rotation -= static_cast<float>(AngularVelocity) * DeltaTime * SpeedModifier;
                AngularVelocity *= -static_cast<float>(SDL_fabs(Bounciness));
            }

            // Apply gravity
            if (UseGravity) {
                Velocity.x += static_cast<float>(Gravity.x) * DeltaTime * SpeedModifier;
                Velocity.y += static_cast<float>(Gravity.y) * DeltaTime * SpeedModifier;
            }
        }
    }

    // Automatically called after each component has already called Tick().
    void PhysicsComponent::LateTick(const double &DeltaTime) {
        // Calls the base class's function
        Component::LateTick(DeltaTime);

        // Move the actor
        if (TickLate) {
            // Apply friction
            GetActor()->Transform.Scale.x *= 1.001;
            GetActor()->Transform.Scale.y *= 1.001;

            if (BrushComponent && BrushComponent->IsColliding()) {
                // Move x
                if (Velocity.x >= 0) {
                    Velocity.x -= static_cast<float>(Friction * DeltaTime * SpeedModifier);
                    if (Velocity.x < 0) {
                        Velocity.x = 0;
                    }
                } else {
                    Velocity.x += static_cast<float>(Friction * DeltaTime * SpeedModifier);
                    if (Velocity.x > 0) {
                        Velocity.x = 0;
                    }
                }

                // Move y
                if (Velocity.y >= 0) {
                    Velocity.y -= static_cast<float>(Friction * DeltaTime * SpeedModifier);
                    if (Velocity.y < 0) {
                        Velocity.y = 0;
                    }
                } else {
                    Velocity.y += static_cast<float>(Friction * DeltaTime * SpeedModifier);
                    if (Velocity.y > 0) {
                        Velocity.y = 0;
                    }
                }

                // Move rotation
                if (AngularVelocity >= 0) {
                    AngularVelocity -= static_cast<float>(Friction * DeltaTime * SpeedModifier);
                    if (AngularVelocity < 0) {
                        AngularVelocity = 0;
                    }
                } else {
                    AngularVelocity += static_cast<float>(Friction * DeltaTime * SpeedModifier);
                    if (AngularVelocity > 0) {
                        AngularVelocity = 0;
                    }
                }
            }

            GetActor()->Transform.Scale.x /= 1.001;
            GetActor()->Transform.Scale.y /= 1.001;

            // Apply velocity and check for collisions
            Reference<FureyEngine::BrushComponent> OtherBrush;
            bool Collision = SDL_fabs(Velocity.x) <= SDL_fabs(Gravity.x) * DeltaTime * 2 &&
                             SDL_fabs(Velocity.y) <= SDL_fabs(Gravity.y) * DeltaTime * 2;

            // Move x
            GetActor()->Transform.Position.x += static_cast<float>(Velocity.x) * DeltaTime * SpeedModifier;
            if (BrushComponent && ((OtherBrush = BrushComponent->IsColliding()))) {
                if (!Collision) {
                    Collision = true;
                    OnCollision(this, static_cast<FureyEngine::BrushComponent *>(OtherBrush));
                    if (auto OtherPhysics = OtherBrush->GetActor()->GetComponent<PhysicsComponent>()) {
                        OtherPhysics->OnCollision(static_cast<PhysicsComponent *>(OtherPhysics),
                                                  static_cast<FureyEngine::BrushComponent *>(BrushComponent));
                    }
                }
                GetActor()->Transform.Position.x -= static_cast<float>(Velocity.x) * DeltaTime * SpeedModifier;
                Velocity.x *= -static_cast<float>(SDL_fabs(Bounciness));
            }

            // Move y
            GetActor()->Transform.Position.y += static_cast<float>(Velocity.y) * DeltaTime * SpeedModifier;
            if (BrushComponent && ((OtherBrush = BrushComponent->IsColliding()))) {
                if (!Collision) {
                    Collision = true;
                    OnCollision(this, static_cast<FureyEngine::BrushComponent *>(OtherBrush));
                    if (auto OtherPhysics = OtherBrush->GetActor()->GetComponent<PhysicsComponent>()) {
                        OtherPhysics->OnCollision(static_cast<PhysicsComponent *>(OtherPhysics),
                                                  static_cast<FureyEngine::BrushComponent *>(BrushComponent));
                    }
                }
                GetActor()->Transform.Position.y -= static_cast<float>(Velocity.y) * DeltaTime * SpeedModifier;
                Velocity.y *= -static_cast<float>(SDL_fabs(Bounciness));
            }

            // Move rotation
            GetActor()->Transform.Rotation += static_cast<float>(AngularVelocity) * DeltaTime * SpeedModifier;
            if (BrushComponent && ((OtherBrush = BrushComponent->IsColliding()))) {
                if (!Collision) {
                    Collision = true;
                    OnCollision(this, static_cast<FureyEngine::BrushComponent *>(OtherBrush));
                    if (auto OtherPhysics = OtherBrush->GetActor()->GetComponent<PhysicsComponent>()) {
                        OtherPhysics->OnCollision(static_cast<PhysicsComponent *>(OtherPhysics),
                                                  static_cast<FureyEngine::BrushComponent *>(BrushComponent));
                    }
                }
                GetActor()->Transform.Rotation -= static_cast<float>(AngularVelocity) * DeltaTime * SpeedModifier;
                AngularVelocity *= -static_cast<float>(SDL_fabs(Bounciness));
            }

            // Apply gravity
            if (UseGravity) {
                Velocity.x += static_cast<float>(Gravity.x) * DeltaTime * SpeedModifier;
                Velocity.y += static_cast<float>(Gravity.y) * DeltaTime * SpeedModifier;
            }
        }
    }

    // Automatically called after this component is removed.
    void PhysicsComponent::Remove() {
        // Calls the base class's function
        Component::Remove();

        // Unbind set active from the actor's set active function
        if (BeginTick) {
            GetActor()->OnSetActive.Unbind(std::bind(PhysicsComponent::SetActive, this, std::placeholders::_1));
        }
    }

    // PHYSICS

    // Returns this actor's current velocity.
    glm::vec2 PhysicsComponent::GetVelocity() const {
        return Velocity;
    }

    // Returns this actor's current angular velocity.
    float PhysicsComponent::GetAngularVelocity() const {
        return AngularVelocity;
    }

    // Sets this actor's current velocity.
    void PhysicsComponent::SetVelocity(const glm::vec2 &Velocity) {
        this->Velocity = Velocity;
    }

    // Sets this actor's current angular velocity.
    void PhysicsComponent::SetAngularVelocity(const float &AngularVelocity) {
        this->AngularVelocity = AngularVelocity;
    }

    // Adds the given force to the actor's velocity and returns it.
    glm::vec2 PhysicsComponent::AddForce(const glm::vec2 &Force) {
        Velocity.x += Force.x;
        Velocity.y += Force.y;
        return Velocity;
    }

    // Adds the given force to the actor's angular velocity and returns it.
    float PhysicsComponent::AddAngularForce(const float &Force) {
        AngularVelocity += Force;
        return AngularVelocity;
    }

    // Zeroes out this actor's velocity and angular velocity.
    void PhysicsComponent::Freeze() {
        Velocity = {0, 0};
        AngularVelocity = 0;
    }

    // Sweeps this actor to the given position and returns where it stopped.
    // This overload uses the given number of steps to check for collision.
    glm::vec2 PhysicsComponent::SweepPositionTo(const glm::vec2 &Position, const int &Steps) {
        if (Steps <= 0) {
            return GetActor()->Transform.Position;
        }

        if (!BrushComponent) {
            return GetActor()->Transform.Position = Position;
        }

        if (auto OtherBrush = BrushComponent->IsColliding()) {
            OnCollision(this, static_cast<FureyEngine::BrushComponent *>(OtherBrush));
            if (auto OtherPhysics = OtherBrush->GetActor()->GetComponent<PhysicsComponent>()) {
                OtherPhysics->OnCollision(static_cast<PhysicsComponent *>(OtherPhysics),
                                          static_cast<FureyEngine::BrushComponent *>(BrushComponent));
            }
            return GetActor()->Transform.Position;
        }

        const float DeltaX = (Position.x - GetActor()->Transform.Position.x) / static_cast<float>(Steps);
        const float DeltaY = (Position.y - GetActor()->Transform.Position.y) / static_cast<float>(Steps);
        for (int i = 0; i < Steps; ++i) {
            GetActor()->Transform.Position.x += DeltaX;
            if (auto OtherBrush = BrushComponent->IsColliding()) {
                OnCollision(this, static_cast<FureyEngine::BrushComponent *>(OtherBrush));
                if (auto OtherPhysics = OtherBrush->GetActor()->GetComponent<PhysicsComponent>()) {
                    OtherPhysics->OnCollision(static_cast<PhysicsComponent *>(OtherPhysics),
                                              static_cast<FureyEngine::BrushComponent *>(BrushComponent));
                }
                GetActor()->Transform.Position.x -= DeltaX;
                return GetActor()->Transform.Position;
            }

            GetActor()->Transform.Position.y += DeltaY;
            if (auto OtherBrush = BrushComponent->IsColliding()) {
                OnCollision(this, static_cast<FureyEngine::BrushComponent *>(OtherBrush));
                if (auto OtherPhysics = OtherBrush->GetActor()->GetComponent<PhysicsComponent>()) {
                    OtherPhysics->OnCollision(static_cast<PhysicsComponent *>(OtherPhysics),
                                              static_cast<FureyEngine::BrushComponent *>(BrushComponent));
                }
                GetActor()->Transform.Position.y -= DeltaY;
                return GetActor()->Transform.Position;
            }
        }

        return GetActor()->Transform.Position;
    }

    // Sweeps this actor to the given position and returns where it stopped.
    // This overload uses a specific distance to calculate the number of steps used to check for collision.
    glm::vec2 PhysicsComponent::SweepPositionTo(const glm::vec2 &Position, const float &StepDistance) {
        if (StepDistance <= 0) {
            return GetActor()->Transform.Position;
        }
        return SweepPositionTo(Position,
                               static_cast<int>(Math::Distance(GetActor()->Transform.Position, Position)
                                                / StepDistance));
    }

    // Sweeps this actor to the given rotation and returns where it stopped.
    // This overload uses the given number of steps to check for collision.
    float PhysicsComponent::SweepRotationTo(const float &Rotation, const int &Steps) {
        if (Steps <= 0) {
            return GetActor()->Transform.Rotation;
        }

        if (!BrushComponent) {
            return GetActor()->Transform.Rotation = Rotation;
        }

        if (auto OtherBrush = BrushComponent->IsColliding()) {
            OnCollision(this, static_cast<FureyEngine::BrushComponent *>(OtherBrush));
            if (auto OtherPhysics = OtherBrush->GetActor()->GetComponent<PhysicsComponent>()) {
                OtherPhysics->OnCollision(static_cast<PhysicsComponent *>(OtherPhysics),
                                          static_cast<FureyEngine::BrushComponent *>(BrushComponent));
            }
            return GetActor()->Transform.Rotation;
        }

        const float Delta = (Rotation - GetActor()->Transform.Rotation) / static_cast<float>(Steps);
        for (int i = 0; i < Steps; ++i) {
            GetActor()->Transform.Rotation += Delta;
            if (auto OtherBrush = BrushComponent->IsColliding()) {
                OnCollision(this, static_cast<FureyEngine::BrushComponent *>(OtherBrush));
                if (auto OtherPhysics = OtherBrush->GetActor()->GetComponent<PhysicsComponent>()) {
                    OtherPhysics->OnCollision(static_cast<PhysicsComponent *>(OtherPhysics),
                                              static_cast<FureyEngine::BrushComponent *>(BrushComponent));
                }
                GetActor()->Transform.Rotation -= Delta;
                return GetActor()->Transform.Rotation;
            }
        }

        return GetActor()->Transform.Rotation;
    }

    // Sweeps this actor to the given rotation and returns where it stopped.
    // This overload uses a specific distance to calculate the number of steps used to check for collision.
    float PhysicsComponent::SweepRotationTo(const float &Rotation, const float &StepDistance) {
        if (StepDistance <= 0) {
            return GetActor()->Transform.Rotation;
        }
        return SweepRotationTo(Rotation,
                               static_cast<int>(SDL_fabs(Rotation - GetActor()->Transform.Rotation)
                                                / StepDistance));
    }

    // Sweeps this actor to the given scale and returns where it stopped.
    // This overload uses the given number of steps to check for collision.
    glm::vec2 PhysicsComponent::SweepScaleTo(const glm::vec2 &Scale, const int &Steps) {
        if (Steps <= 0) {
            return GetActor()->Transform.Scale;
        }

        if (!BrushComponent) {
            return GetActor()->Transform.Scale = Scale;
        }

        if (auto OtherBrush = BrushComponent->IsColliding()) {
            OnCollision(this, static_cast<FureyEngine::BrushComponent *>(OtherBrush));
            if (auto OtherPhysics = OtherBrush->GetActor()->GetComponent<PhysicsComponent>()) {
                OtherPhysics->OnCollision(static_cast<PhysicsComponent *>(OtherPhysics),
                                          static_cast<FureyEngine::BrushComponent *>(BrushComponent));
            }
            return GetActor()->Transform.Scale;
        }

        const float DeltaX = (Scale.x - GetActor()->Transform.Scale.x) / static_cast<float>(Steps);
        const float DeltaY = (Scale.y - GetActor()->Transform.Scale.y) / static_cast<float>(Steps);
        for (int i = 0; i < Steps; ++i) {
            GetActor()->Transform.Scale.x += DeltaX;
            if (auto OtherBrush = BrushComponent->IsColliding()) {
                OnCollision(this, static_cast<FureyEngine::BrushComponent *>(OtherBrush));
                if (auto OtherPhysics = OtherBrush->GetActor()->GetComponent<PhysicsComponent>()) {
                    OtherPhysics->OnCollision(static_cast<PhysicsComponent *>(OtherPhysics),
                                              static_cast<FureyEngine::BrushComponent *>(BrushComponent));
                }
                GetActor()->Transform.Scale.x -= DeltaX;
                return GetActor()->Transform.Scale;
            }

            GetActor()->Transform.Scale.y += DeltaY;
            if (auto OtherBrush = BrushComponent->IsColliding()) {
                OnCollision(this, static_cast<FureyEngine::BrushComponent *>(OtherBrush));
                if (auto OtherPhysics = OtherBrush->GetActor()->GetComponent<PhysicsComponent>()) {
                    OtherPhysics->OnCollision(static_cast<PhysicsComponent *>(OtherPhysics),
                                              static_cast<FureyEngine::BrushComponent *>(BrushComponent));
                }
                GetActor()->Transform.Scale.y -= DeltaY;
                return GetActor()->Transform.Scale;
            }
        }

        return GetActor()->Transform.Scale;
    }

    // Sweeps this actor to the given scale and returns where it stopped.
    // This overload uses a specific distance to calculate the number of steps used to check for collision.
    glm::vec2 PhysicsComponent::SweepScaleTo(const glm::vec2 &Scale, const float &StepDistance) {
        if (StepDistance <= 0) {
            return GetActor()->Transform.Scale;
        }
        return SweepPositionTo(Scale,
                               static_cast<int>(Math::Distance(GetActor()->Transform.Position, Scale)
                                                / StepDistance));
    }
}
