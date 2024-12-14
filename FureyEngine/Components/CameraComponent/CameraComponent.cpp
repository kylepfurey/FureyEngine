// .cpp
// Camera Component Class Script
// by Kyle Furey

#include "CameraComponent.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Constructs a new camera component.
    CameraComponent::CameraComponent(
        const glm::vec2 &Offset,
        const glm::vec2 &FollowSpeed,
        const glm::vec2 &ClampMin,
        const glm::vec2 &ClampMax,
        const std::set<std::string> &TargetRenderers,
        const bool &TickLate)
        : Component("Camera", true),
          Offset(Offset),
          FollowSpeed(FollowSpeed),
          ClampMin(ClampMin),
          ClampMax(ClampMax),
          TargetRenderers(TargetRenderers),
          TickLate(TickLate) {
    }

    // EVENTS

    // Automatically called before each component has already called Start().
    void CameraComponent::Attach() {
        // Calls the base class's function
        Component::Attach();

        // Bind set active to the actor's set active function
        GetActor()->OnSetActive.Bind(std::bind(CameraComponent::SetActive, this, std::placeholders::_1));

        // Set the camera's position
        SnapPosition();
    }

    // Automatically called each world tick.
    void CameraComponent::Tick(const double &DeltaTime) {
        // Calls the base class's function
        Component::Tick(DeltaTime);

        // Update the camera's position
        if (!TickLate) {
            for (const auto &Renderer: TargetRenderers) {
                auto &Camera = Resources::Renderers[Renderer];
                Camera.Position.x = Math::Lerp(Camera.Position.x, GetActor()->Transform.Position.x + Offset.x,
                                               static_cast<float>(FollowSpeed.x >= 0 ? FollowSpeed.x * DeltaTime : 1));
                Camera.Position.y = Math::Lerp(Camera.Position.y, GetActor()->Transform.Position.y + Offset.y,
                                               static_cast<float>(FollowSpeed.y >= 0 ? FollowSpeed.y * DeltaTime : 1));
                Camera.Position.x = ClampMin.x > ClampMax.x
                                        ? Camera.Position.x
                                        : SDL_clamp(Camera.Position.x, ClampMin.x, ClampMax.x);

                Camera.Position.y = ClampMin.y > ClampMax.y
                                        ? Camera.Position.y
                                        : SDL_clamp(Camera.Position.y, ClampMin.y, ClampMax.y);
            }
        }
    }

    // Automatically called after each component has already called Tick().
    void CameraComponent::LateTick(const double &DeltaTime) {
        // Calls the base class's function
        Component::LateTick(DeltaTime);

        // Update the camera's position
        if (TickLate) {
            for (const auto &Renderer: TargetRenderers) {
                auto &Camera = Resources::Renderers[Renderer];
                Camera.Position.x = Math::Lerp(Camera.Position.x, GetActor()->Transform.Position.x + Offset.x,
                                               static_cast<float>(FollowSpeed.x >= 0 ? FollowSpeed.x * DeltaTime : 1));
                Camera.Position.y = Math::Lerp(Camera.Position.y, GetActor()->Transform.Position.y + Offset.y,
                                               static_cast<float>(FollowSpeed.y >= 0 ? FollowSpeed.y * DeltaTime : 1));
                Camera.Position.x = ClampMin.x > ClampMax.x
                                        ? Camera.Position.x
                                        : SDL_clamp(Camera.Position.x, ClampMin.x, ClampMax.x);

                Camera.Position.y = ClampMin.y > ClampMax.y
                                        ? Camera.Position.y
                                        : SDL_clamp(Camera.Position.y, ClampMin.y, ClampMax.y);
            }
        }
    }

    // Automatically called after this component is removed.
    void CameraComponent::Remove() {
        // Calls the base class's function
        Component::Remove();

        // Unbind set active from the actor's set active function
        if (BeginTick) {
            GetActor()->OnSetActive.Unbind(std::bind(CameraComponent::SetActive, this, std::placeholders::_1));
        }
    }


    // CAMERA

    // Immediately updates the renderers' positions to this component's actor's position.
    void CameraComponent::SnapPosition() const {
        for (const auto &Renderer: TargetRenderers) {
            auto &Camera = Resources::Renderers[Renderer];
            Camera.Position = GetActor()->Transform.Position;
            Camera.Position.x += Offset.x;
            Camera.Position.y += Offset.y;
            Camera.Position.x = ClampMin.x > ClampMax.x
                                    ? Camera.Position.x
                                    : SDL_clamp(Camera.Position.x, ClampMin.x, ClampMax.x);

            Camera.Position.y = ClampMin.y > ClampMax.y
                                    ? Camera.Position.y
                                    : SDL_clamp(Camera.Position.y, ClampMin.y, ClampMax.y);
        }
    }

    // Calculates the world position at the center of the screen (originating from the actor).
    glm::vec2 CameraComponent::Center() const {
        return {
            GetActor()->Transform.Position.x + Offset.x,
            GetActor()->Transform.Position.y + Offset.y
        };
    }

    // Calculates the world position at the bottom left of the camera (originating from the actor).
    glm::vec2 CameraComponent::BottomLeft() const {
        return {
            GetActor()->Transform.Position.x - static_cast<float>(SCREEN_WIDTH) / 2
            + Offset.x,
            GetActor()->Transform.Position.y - static_cast<float>(SCREEN_WIDTH) / TARGET_RESOLUTION_RATIO / 2
            + Offset.y
        };
    }

    // Calculates the world position at the bottom right of the camera (originating from the actor).
    glm::vec2 CameraComponent::BottomRight() const {
        return {
            GetActor()->Transform.Position.x + static_cast<float>(SCREEN_WIDTH) / 2
            + Offset.x,
            GetActor()->Transform.Position.y - static_cast<float>(SCREEN_WIDTH) / TARGET_RESOLUTION_RATIO / 2
            + Offset.y
        };
    }

    // Calculates the world position at the top left of the camera (originating from the actor).
    glm::vec2 CameraComponent::TopLeft() const {
        return {
            GetActor()->Transform.Position.x - static_cast<float>(SCREEN_WIDTH) / 2
            + Offset.x,
            GetActor()->Transform.Position.y + static_cast<float>(SCREEN_WIDTH) / TARGET_RESOLUTION_RATIO / 2
            + Offset.y
        };
    }

    // Calculates the world position at the top right of the camera (originating from the actor).
    glm::vec2 CameraComponent::TopRight() const {
        return {
            GetActor()->Transform.Position.x + static_cast<float>(SCREEN_WIDTH) / 2
            + Offset.x,
            GetActor()->Transform.Position.y + static_cast<float>(SCREEN_WIDTH) / TARGET_RESOLUTION_RATIO / 2
            + Offset.y
        };
    }

    // Calculates a world position from the given screen position (relative to the bottom left of the screen).
    glm::vec2 CameraComponent::GetWorldPosition(const glm::vec2 &ScreenPosition) const {
        const auto Min = BottomLeft();
        const auto Max = TopRight();
        return {
            Math::Lerp(Min.x, Max.x, ScreenPosition.x),
            Math::Lerp(Min.y, Max.y, ScreenPosition.y)
        };
    }

    // Calculates a screen position from the given world position (relative to the bottom left of the screen).
    glm::vec2 CameraComponent::GetScreenPosition(const glm::vec2 &WorldPosition) const {
        const auto Min = BottomLeft();
        const auto Max = TopRight();
        return {
            Math::InverseLerp(Min.x, Max.x, WorldPosition.x),
            Math::InverseLerp(Min.y, Max.y, WorldPosition.y)
        };
    }

    // Calculates the mouse's current world position.
    glm::vec2 CameraComponent::GetMouseWorldPosition(const std::string &Window) const {
        return GetWorldPosition(GetMouseScreenPosition(Window));
    }

    // Calculates the mouse's current screen position (relative to the bottom left of the screen).
    glm::vec2 CameraComponent::GetMouseScreenPosition(const std::string &Window) const {
        if (Resources::Windows.Contains(Window)) {
            const glm::vec2 WindowSize = Resources::Windows[Window].Size();
            glm::vec2 MousePosition = Input::GetMousePosition();
            MousePosition.y -= WindowSize.y;
            MousePosition.x /= WindowSize.x;
            MousePosition.y /= -WindowSize.y;
            return MousePosition;
        }
        return {0, 0};
    }

    // Returns whether the given point is in the view of the camera (originating from the actor).
    bool CameraComponent::IsInView(const glm::vec2 &WorldPosition) const {
        const auto Min = BottomLeft();
        const auto Max = TopRight();
        return WorldPosition.x >= Min.x && WorldPosition.x <= Max.x &&
               WorldPosition.y >= Min.y && WorldPosition.y <= Max.y;
    }
}
