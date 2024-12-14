// .h
// Camera Component Class Script
// by Kyle Furey

#pragma once
#include "../../Engine/Engine.h"

namespace FureyEngine {
    /** A component that forces renderers to follow the component's actor's position. */
    class CameraComponent : public Component {
    public:
        // CAMERA

        /** An offset to apply to the camera relative to the component's actor. */
        glm::vec2 Offset = {0, 0};

        /**
         * The speed to follow the component's actor at.<br/>
         * NOTE: A negative speed is an instantaneous.
         */
        glm::vec2 FollowSpeed = {3, 3};

        /**
         * The negative clamps of this camera (in world space).<br/>
         * NOTE: A clamp greater than its maximum clamp is ignored.
         */
        glm::vec2 ClampMin = {1, 1};

        /**
         * The positive clamps of this camera (in world space).<br/>
         * NOTE: A clamp less than minimum clamp is ignored.
         */
        glm::vec2 ClampMax = {-1, -1};

        /** This camera's target renderers. */
        std::set<std::string> TargetRenderers = {"main"};

        /** Whether to use LateTick() instead of Tick(). */
        bool TickLate = true;


        // CONSTRUCTORS

        /** Constructs a new camera component. */
        explicit CameraComponent(
            const glm::vec2 &Offset = {0, 0},
            const glm::vec2 &FollowSpeed = {3, 3},
            const glm::vec2 &ClampMin = {1, 1},
            const glm::vec2 &ClampMax = {-1, -1},
            const std::set<std::string> &TargetRenderers = {"main"},
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

        // CAMERA

        /** Immediately updates the renderers' positions to this component's actor's position. */
        virtual void SnapPosition() const;

        /** Calculates the world position at the center of the screen (originating from the actor). */
        [[nodiscard]] virtual glm::vec2 Center() const;

        /** Calculates the world position at the bottom left of the screen (originating from the actor). */
        [[nodiscard]] virtual glm::vec2 BottomLeft() const;

        /** Calculates the world position at the bottom right of the screen (originating from the actor). */
        [[nodiscard]] virtual glm::vec2 BottomRight() const;

        /** Calculates the world position at the top left of the screen (originating from the actor). */
        [[nodiscard]] virtual glm::vec2 TopLeft() const;

        /** Calculates the world position at the top right of the screen (originating from the actor). */
        [[nodiscard]] virtual glm::vec2 TopRight() const;

        /** Calculates a world position from the given screen position (relative to the bottom left of the screen). */
        [[nodiscard]] virtual glm::vec2 GetWorldPosition(const glm::vec2 &ScreenPosition) const;

        /** Calculates a screen position from the given world position (relative to the bottom left of the screen). */
        [[nodiscard]] virtual glm::vec2 GetScreenPosition(const glm::vec2 &WorldPosition) const;

        /** Calculates the mouse's current world position. */
        [[nodiscard]] virtual glm::vec2 GetMouseWorldPosition(const std::string &Window = "main") const;

        /** Calculates the mouse's current screen position (relative to the bottom left of the screen). */
        [[nodiscard]] virtual glm::vec2 GetMouseScreenPosition(const std::string &Window = "main") const;

        /** Returns whether the given point is in the view of the camera (originating from the actor). */
        [[nodiscard]] virtual bool IsInView(const glm::vec2 &WorldPosition) const;
    };
}
