// .h
// Brush Collision Component Class Script
// by Kyle Furey

#pragma once
#include "../AnimationComponent/AnimationComponent.h"

#define FIRE_WHILE_COLLIDING 1

namespace FureyEngine {
    /**
     * A component used to generate a bounding box around an actor that can collide with other brushes.<br/>
     * Active brushes can send and receive collision responses with other brushes but brushes marked as triggers will only receive collision responses.
     */
    class BrushComponent : public Component {
    public:
        // BRUSH

        /** Each type of collision that may occur from a brush component. */
        enum class CollisionState {
            /** No collision is currently occurring. */
            NONE = 0,

            /** Called the tick when collision occurs. */
            COLLISION_ENTER = 1,

            /** Called each tick collision occurs after the tick it was entered. */
            COLLISION_STAY = 2,

            /** Called the tick when collision stops. */
            COLLISION_EXIT = 3
        };

        /**
         * Each of 4 edges that may be overlapped by another brush.<br/>
         * This may be used as bitflags for each overlapped edge.
         */
        enum class OverlappingEdges {
            /** No edges are overlapped. */
            NONE = 0,

            /** The very top of the brush. */
            TOP = 1,

            /** The very bottom of the brush. */
            BOTTOM = 2,

            /** The leftmost side of the brush. */
            LEFT = 4,

            /** The rightmost side of the brush. */
            RIGHT = 8
        };

    protected:
        // BRUSH

        /** Each instantiated brush component in each world. */
        static std::map<World *, std::set<BrushComponent *> > AllBrushes;

        /** The current state of this brush's collision with other brushes. */
        std::map<BrushComponent *, CollisionState> CollisionStates;

    public:
        // BRUSH

        /** The transform that represents this brush's bounds relative to the actor. */
        Transform Bounds;

        /** Whether this brush is a trigger and will only receive collision responses. */
        bool Trigger = false;

        /**
         * A delegate to call when this brush collides with another brush.<br/>
         * The first parameter is a pointer to this brush component.<br/>
         * The second parameter is a pointer to the brush component that was collided with.
         */
        Delegate<BrushComponent *, BrushComponent *> OnCollision;

        /**
         * A layer used to separate specific types of collisions.<br/>
         * Layer "All" is reserved for all collision regardless of layer name.
         */
        std::string Layer = "All";

        /** Whether to use LateTick() instead of Tick(). */
        bool TickLate = true;


        // CONSTRUCTORS

        /** Constructs a new brush component. */
        explicit BrushComponent(const Transform &Bounds,
                                const bool &Trigger = false,
                                std::string Layer = "All",
                                const bool &TickLate = true);

        /** Constructs a new brush component scaled to match the given render component's texture. */
        explicit BrushComponent(const Reference<RenderComponent> &RenderComponent = nullref,
                                const bool &Trigger = false,
                                std::string Layer = "All",
                                const bool &TickLate = true);

        /** Constructs a new brush component scaled to match the given animation component's texture. */
        explicit BrushComponent(const Reference<AnimationComponent> &AnimationComponent = nullref,
                                const bool &Trigger = false,
                                std::string Layer = "All",
                                const bool &TickLate = true);

        // DESTRUCTOR

        /** Destroys this brush component. */
        ~BrushComponent() override;

        // EVENTS

        /** Automatically called before each component has already called Start(). */
        void Attach() override;

        /** Automatically called when this component is first created. */
        void Start() override;

        /** Automatically called each world tick. */
        void Tick(const double &DeltaTime) override;

        /** Automatically called after each component has already called Tick(). */
        void LateTick(const double &DeltaTime) override;

        // ACTIVITY

        /**
         * Sets whether this component is active.<br/>
         * This can be overridden to control custom functionality.
         */
        void SetActive(const bool &Active) override;

        // GETTERS

        /** Calculates the world position at the center of the brush relative to its actor's bounds. */
        [[nodiscard]] virtual glm::vec2 Center() const;

        /** Calculates the world position at the bottom left of the brush relative to its actor's bounds. */
        [[nodiscard]] virtual glm::vec2 BottomLeft() const;

        /** Calculates the world position at the bottom right of the brush relative to its actor's bounds. */
        [[nodiscard]] virtual glm::vec2 BottomRight() const;

        /** Calculates the world position at the top left of the brush relative to its actor's bounds. */
        [[nodiscard]] virtual glm::vec2 TopLeft() const;

        /** Calculates the world position at the top right of the brush relative to its actor's bounds. */
        [[nodiscard]] virtual glm::vec2 TopRight() const;

        /** Returns whether the given world position is within the bounds of this brush. */
        [[nodiscard]] virtual bool IsWithinBounds(const glm::vec2 &WorldPosition) const;

        /** Returns whether the given edge is overlapping the given brush. */
        [[nodiscard]] virtual bool IsOverlappingEdge(const Reference<BrushComponent> &OtherBrush,
                                                     const OverlappingEdges &OverlappedEdge) const;

        /** Returns each of this brush's edges that the given brush is overlapping. */
        [[nodiscard]] virtual OverlappingEdges GetOverlappedEdges(const Reference<BrushComponent> &OtherBrush) const;

        /** Returns the state of collision between this brush and the given brush. */
        [[nodiscard]] virtual CollisionState CollisionStateWith(Reference<BrushComponent> OtherBrush) const;

        /** Returns whether the given brush is currently overlapping this brush. */
        [[nodiscard]] virtual bool IsOverlapping(const Reference<BrushComponent> &OtherBrush) const;

        /** Returns the first brush found that this brush is currently being overlapped by (not including triggers). */
        [[nodiscard]] virtual Reference<BrushComponent> IsColliding() const;
    };
}
