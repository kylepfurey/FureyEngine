// .cpp
// Brush Collision Component Class Script
// by Kyle Furey

#include "BrushComponent.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Constructs a new brush component.
    BrushComponent::BrushComponent(const Transform &Bounds,
                                   const bool &Trigger,
                                   std::string Layer,
                                   const bool &TickLate)
        : Component("Brush", true), Bounds(Bounds), Trigger(Trigger), Layer(std::move(Layer)), TickLate(TickLate) {
    }

    // Constructs a new brush component scaled to match the given render component's texture.
    BrushComponent::BrushComponent(const Reference<RenderComponent> &RenderComponent,
                                   const bool &Trigger,
                                   std::string Layer,
                                   const bool &TickLate)
        : Component("Brush", true), Trigger(Trigger), Layer(std::move(Layer)), TickLate(TickLate) {
        if (RenderComponent && RenderComponent->GetImage() != nullptr) {
            Bounds.Scale = Renderer::PixelsToWorld(RenderComponent->GetImage()->TextureSize());
        } else {
            Bounds.Scale.x = FLT_MAX;
            Bounds.Scale.y = FLT_MAX;
        }
    }

    // Constructs a new brush component scaled to match the given animation component's texture.
    BrushComponent::BrushComponent(const Reference<AnimationComponent> &AnimationComponent,
                                   const bool &Trigger,
                                   std::string Layer,
                                   const bool &TickLate)
        : Component("Brush", true), Trigger(Trigger), Layer(std::move(Layer)), TickLate(TickLate) {
        if (AnimationComponent && AnimationComponent->GetCurrentAnimation()->Animation != nullptr) {
            Bounds.Scale = Renderer::PixelsToWorld(
                AnimationComponent->GetCurrentAnimation()->Animation->Frame(0)->TextureSize());
        }
    }

    // DESTRUCTOR

    // Destroys this brush component.
    BrushComponent::~BrushComponent() {
        // Removes this brush from the set of all brushes
        AllBrushes[static_cast<World *>(GetActor()->GetWorld())].erase(this);
        if (AllBrushes[static_cast<World *>(GetActor()->GetWorld())].empty()) {
            AllBrushes.erase(static_cast<World *>(GetActor()->GetWorld()));
        }
    }

    // Automatically called before each component has already called Start().
    void BrushComponent::Attach() {
        // Calls the base class's function
        Component::Attach();

        // Adds this brush to the set of all brushes
        AllBrushes[static_cast<World *>(GetActor()->GetWorld())].insert(this);

        // Finds a render component if one was not valid
        if (Bounds.Scale.x == FLT_MAX && Bounds.Scale.y == FLT_MAX) {
            const auto RenderComponent = GetActor()->GetComponent<FureyEngine::RenderComponent>();
            Bounds.Scale = RenderComponent && RenderComponent->GetImage() != nullptr
                               ? Renderer::PixelsToWorld(RenderComponent->GetImage()->TextureSize())
                               : glm::vec2{0, 0};
        }
    }

    // Automatically called when this component is first created.
    void BrushComponent::Start() {
        // Calls the base class's function
        Component::Start();

        // Update collision responses
        for (const auto &Element: AllBrushes[static_cast<World *>(GetActor()->GetWorld())]) {
            if (Element == this) {
                continue;
            }

            if (const auto OtherBrush = Reference(Element);
                OtherBrush->IsActive() && !OtherBrush->Trigger && (Layer == "All" || Layer == OtherBrush->Layer) &&
                IsOverlapping(OtherBrush)) {
                CollisionStates[Element] = CollisionState::COLLISION_EXIT;
                OnCollision(this, Element);
            }
        }
    }

    // Automatically called each world tick.
    void BrushComponent::Tick(const double &DeltaTime) {
        // Calls the base class's function
        Component::Tick(DeltaTime);

        // Update collision responses
        if (!TickLate) {
            for (const auto &Element: AllBrushes[static_cast<World *>(GetActor()->GetWorld())]) {
                if (Element == this) {
                    continue;
                }

                if (const auto OtherBrush = Reference(Element);
                    OtherBrush->IsActive() && !OtherBrush->Trigger && (Layer == "All" || Layer == OtherBrush->Layer) &&
                    IsOverlapping(OtherBrush)) {
                    if (CollisionStates.count(Element)) {
#if FIRE_WHILE_COLLIDING
                        CollisionStates[Element] = CollisionState::COLLISION_STAY;
                        OnCollision(this, Element);
#endif
                    } else {
                        CollisionStates[Element] = CollisionState::COLLISION_ENTER;
                        OnCollision(this, Element);
                    }
                } else {
                    if (CollisionStates.count(Element)) {
                        if (CollisionStates[Element] == CollisionState::COLLISION_EXIT) {
                            CollisionStates.erase(Element);
                        } else {
                            CollisionStates[Element] = CollisionState::COLLISION_EXIT;
                            OnCollision(this, Element);
                        }
                    }
                }
            }
        }
    }

    // Automatically called after each component has already called Tick().
    void BrushComponent::LateTick(const double &DeltaTime) {
        // Calls the base class's function
        Component::LateTick(DeltaTime);

        // Update collision responses
        if (TickLate) {
            for (const auto &Element: AllBrushes[static_cast<World *>(GetActor()->GetWorld())]) {
                if (Element == this) {
                    continue;
                }

                if (const auto OtherBrush = Reference(Element);
                    OtherBrush->IsActive() && !OtherBrush->Trigger && (Layer == "All" || Layer == OtherBrush->Layer) &&
                    IsOverlapping(OtherBrush)) {
                    if (CollisionStates.count(Element)) {
#if FIRE_WHILE_COLLIDING
                        CollisionStates[Element] = CollisionState::COLLISION_STAY;
                        OnCollision(this, Element);
#endif
                    } else {
                        CollisionStates[Element] = CollisionState::COLLISION_ENTER;
                        OnCollision(this, Element);
                    }
                } else {
                    if (CollisionStates.count(Element)) {
                        if (CollisionStates[Element] == CollisionState::COLLISION_EXIT) {
                            CollisionStates.erase(Element);
                        } else {
                            CollisionStates[Element] = CollisionState::COLLISION_EXIT;
                            OnCollision(this, Element);
                        }
                    }
                }
            }
        }
    }

    // ACTIVITY

    // Sets whether this component is active.
    // This can be overridden to control custom functionality.
    void BrushComponent::SetActive(const bool &Active) {
        // Calls the base class's function
        Component::SetActive(Active);

        // Clears all collision states
        if (!Active) {
            for (const auto &[Brush, State]: CollisionStates) {
                CollisionStates[Brush] = CollisionState::COLLISION_EXIT;
                OnCollision(this, Brush);
            }
            CollisionStates.clear();
        }
    }

    // GETTERS

    // Calculates the world position at the center of the brush relative to its actor's bounds.
    glm::vec2 BrushComponent::Center() const {
        return Transform::RelativeToWorld(Transform(),
                                          Transform::RelativeToWorld(Bounds,
                                                                     GetActor()->Transform)).Position;
    }

    // Calculates the world position at the bottom left of the brush.
    glm::vec2 BrushComponent::BottomLeft() const {
        return Transform::RelativeToWorld(Transform({-0.5, -0.5}),
                                          Transform::RelativeToWorld(Bounds,
                                                                     GetActor()->Transform)).Position;
    }

    // Calculates the world position at the bottom right of the brush.
    glm::vec2 BrushComponent::BottomRight() const {
        return Transform::RelativeToWorld(Transform({0.5, -0.5}),
                                          Transform::RelativeToWorld(Bounds,
                                                                     GetActor()->Transform)).Position;
    }

    // Calculates the world position at the top left of the brush.
    glm::vec2 BrushComponent::TopLeft() const {
        return Transform::RelativeToWorld(Transform({-0.5, 0.5}),
                                          Transform::RelativeToWorld(Bounds,
                                                                     GetActor()->Transform)).Position;
    }

    // Calculates the world position at the top right of the brush.
    glm::vec2 BrushComponent::TopRight() const {
        return Transform::RelativeToWorld(Transform({0.5, 0.5}),
                                          Transform::RelativeToWorld(Bounds,
                                                                     GetActor()->Transform)).Position;
    }

    // Returns whether the given world position is within the bounds of this brush.
    bool BrushComponent::IsWithinBounds(const glm::vec2 &WorldPosition) const {
        if (Bounds.Scale == glm::vec2{0, 0}) {
            return false;
        }

        const auto RelativeTransform = Transform::WorldToRelative(Transform(WorldPosition),
                                                                  Transform::RelativeToWorld(
                                                                      Bounds, GetActor()->Transform));
        return SDL_fabs(RelativeTransform.Position.x) <= 0.5 &&
               SDL_fabs(RelativeTransform.Position.y) <= 0.5;
    }

    // Returns whether the given edge is overlapping the given brush.
    bool BrushComponent::IsOverlappingEdge(const Reference<BrushComponent> &OtherBrush,
                                           const OverlappingEdges &OverlappedEdge) const {
        if (!OtherBrush || OtherBrush->Bounds.Scale == glm::vec2{0, 0}) {
            return false;
        }

        glm::vec2 PointA;
        glm::vec2 PointB;

        switch (OverlappedEdge) {
            default:
                return false;

            case OverlappingEdges::TOP:
                PointA = TopLeft();
                PointB = TopRight();
                break;

            case OverlappingEdges::BOTTOM:
                PointA = BottomLeft();
                PointB = BottomRight();
                break;

            case OverlappingEdges::LEFT:
                PointA = BottomLeft();
                PointB = TopLeft();
                break;

            case OverlappingEdges::RIGHT:
                PointA = BottomRight();
                PointB = TopRight();
                break;
        }

        return Math::DoesLineOverlapLine(PointA, PointB,
                                         OtherBrush->BottomLeft(), OtherBrush->BottomRight()) ||
               Math::DoesLineOverlapLine(PointA, PointB,
                                         OtherBrush->TopLeft(), OtherBrush->TopRight()) ||
               Math::DoesLineOverlapLine(PointA, PointB,
                                         OtherBrush->BottomLeft(), OtherBrush->TopLeft()) ||
               Math::DoesLineOverlapLine(PointA, PointB,
                                         OtherBrush->BottomRight(), OtherBrush->TopRight());
    }

    // Returns each of this brush's edges that the given brush is overlapping.
    BrushComponent::OverlappingEdges BrushComponent::GetOverlappedEdges(
        const Reference<BrushComponent> &OtherBrush) const {
        if (!OtherBrush || OtherBrush->Bounds.Scale == glm::vec2{0, 0}) {
            return OverlappingEdges::NONE;
        }

        auto OverlappedEdges = OverlappingEdges::NONE;

        // Check the top edge
        if (IsOverlappingEdge(OtherBrush, OverlappingEdges::TOP)) {
            OverlappedEdges = Math::AddEnumFlag(OverlappedEdges, OverlappingEdges::TOP);
        }

        // Check the top edge
        if (IsOverlappingEdge(OtherBrush, OverlappingEdges::BOTTOM)) {
            OverlappedEdges = Math::AddEnumFlag(OverlappedEdges, OverlappingEdges::BOTTOM);
        }

        // Check the left edge
        if (IsOverlappingEdge(OtherBrush, OverlappingEdges::LEFT)) {
            OverlappedEdges = Math::AddEnumFlag(OverlappedEdges, OverlappingEdges::LEFT);
        }

        // Check the right edge
        if (IsOverlappingEdge(OtherBrush, OverlappingEdges::RIGHT)) {
            OverlappedEdges = Math::AddEnumFlag(OverlappedEdges, OverlappingEdges::RIGHT);
        }

        return OverlappedEdges;
    }

    // Returns the state of collision between this brush and the given brush.
    BrushComponent::CollisionState BrushComponent::CollisionStateWith(Reference<BrushComponent> OtherBrush) const {
        if (!OtherBrush) {
            return CollisionState::NONE;
        }
        const auto Brush = static_cast<BrushComponent *>(OtherBrush);
        return CollisionStates.count(Brush)
                   ? CollisionStates.at(Brush)
                   : CollisionState::NONE;
    }

    // Returns whether the given brush is currently overlapping this brush.
    bool BrushComponent::IsOverlapping(const Reference<BrushComponent> &OtherBrush) const {
        if (Bounds.Scale == glm::vec2{0, 0}) {
            return false;
        }

        return GetOverlappedEdges(OtherBrush) != OverlappingEdges::NONE ||
               IsWithinBounds(OtherBrush->BottomLeft()) ||
               IsWithinBounds(OtherBrush->BottomRight()) ||
               IsWithinBounds(OtherBrush->TopLeft()) ||
               IsWithinBounds(OtherBrush->TopRight());
    }

    // Returns the first brush found that this brush is currently being overlapped by (not including triggers).
    Reference<BrushComponent> BrushComponent::IsColliding() const {
        if (Bounds.Scale == glm::vec2{0, 0} || !IsActive()) {
            return nullref;
        }

        for (const auto &Element: AllBrushes[static_cast<World *>(GetActor()->GetWorld())]) {
            if (Element == this || Element->Trigger) {
                continue;
            }
            if (const auto OtherBrush = Reference(Element); IsOverlapping(OtherBrush)) {
                return OtherBrush;
            }
        }
        return nullref;
    }

    // STATIC VARIABLE INITIALIZATION

    // Each instantiated brush component in each world.
    std::map<World *, std::set<BrushComponent *> > BrushComponent::AllBrushes;
}
