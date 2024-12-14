// .cpp
// Image Render Component Class Script
// by Kyle Furey

#include "RenderComponent.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Constructs a new render component for this actor.
    RenderComponent::RenderComponent(Image *Image, const RenderMode &RenderMode, const bool &RenderLast)
        : Component("Render", true), MyImage(Image), MyRenderMode(RenderMode), RenderingLast(RenderLast) {
        MyTexture = Texture(0, {0, 0}, nullptr);
    }

    // EVENTS

    // Automatically called before each component has already called Start().
    void RenderComponent::Attach() {
        // Calls the base class's function
        Component::Attach();

        // Bind set active to the actor's set active function
        GetActor()->OnSetActive.Bind(std::bind(RenderComponent::SetActive, this, std::placeholders::_1));

        // Set the texture's transform to this actor
        MyTexture.Transform = &GetActor()->Transform;

        // Calls SetActive() to initialize the texture
        SetActive(IsActive());
    }

    // Automatically called after this component is removed.
    void RenderComponent::Remove() {
        // Calls the base class's function
        Component::Remove();

        // Unbind set active from the actor's set active function
        GetActor()->OnSetActive.Unbind(std::bind(RenderComponent::SetActive, this, std::placeholders::_1));

        // Call SetActive() to clean up the texture
        SetActive(false);
    }

    // ACTIVITY

    // Sets whether this component is active.
    // This can be overridden to control custom functionality.
    void RenderComponent::SetActive(const bool &Active) {
        // Calls the base class's function
        Component::SetActive(Active);

        if (Active) {
            if (MyTexture.ID == 0 && MyImage != nullptr) {
                MyTexture = {MyImage->TextureID(), MyImage->TextureSize(), MyTexture.Transform};
                if (MyRenderMode == RenderMode::DYNAMIC_TEXTURE) {
                    for (const auto &Renderer: TargetRenderers) {
                        if (RenderingLast) {
                            Resources::Renderers[Renderer].DynamicTextures.push_back(&MyTexture);
                        } else {
                            Resources::Renderers[Renderer].DynamicTextures.push_front(&MyTexture);
                        }
                    }
                } else {
                    for (const auto &Renderer: TargetRenderers) {
                        if (RenderingLast) {
                            Resources::Renderers[Renderer].StaticTextures.push_back(&MyTexture);
                        } else {
                            Resources::Renderers[Renderer].StaticTextures.push_front(&MyTexture);
                        }
                    }
                }
            }
        } else {
            if (MyTexture.ID != 0) {
                if (MyRenderMode == RenderMode::DYNAMIC_TEXTURE) {
                    for (const auto &Renderer: TargetRenderers) {
                        auto Iterator = Resources::Renderers[Renderer].DynamicTextures.begin();
                        for (const auto &Texture: Resources::Renderers[Renderer].DynamicTextures) {
                            if (Texture == &MyTexture) {
                                Resources::Renderers[Renderer].DynamicTextures.erase(Iterator);
                                break;
                            }
                            ++Iterator;
                        }
                    }
                } else {
                    for (const auto &Renderer: TargetRenderers) {
                        auto Iterator = Resources::Renderers[Renderer].StaticTextures.begin();
                        for (const auto &Texture: Resources::Renderers[Renderer].StaticTextures) {
                            if (Texture == &MyTexture) {
                                Resources::Renderers[Renderer].StaticTextures.erase(Iterator);
                                break;
                            }
                            ++Iterator;
                        }
                    }
                }
                MyTexture = {0, {0, 0}, MyTexture.Transform};
            }
        }
    }

    // GETTERS

    // Returns the render mode of this renderer.
    RenderMode RenderComponent::GetRenderMode() const {
        return MyRenderMode;
    }

    // Returns the image of this renderer.
    Image *RenderComponent::GetImage() const {
        return MyImage;
    }

    // Returns the names of the target renderers of this renderer.
    std::set<std::string> RenderComponent::GetTargetRenderers() const {
        return TargetRenderers;
    }

    // Returns the current generated texture of this renderer.
    Texture RenderComponent::GetTexture() const {
        return MyTexture;
    }

    // Returns whether this texture was recently set to render first in the queue and therefore behind other textures.
    bool RenderComponent::IsRenderedFirst() const {
        return !RenderingLast;
    }

    // Returns whether this texture was recently set to render last in the queue and therefore in front of other textures.
    bool RenderComponent::IsRenderedLast() const {
        return RenderingLast;
    }

    // SETTERS

    // Sets the render mode of this renderer.
    void RenderComponent::SetRenderMode(const RenderMode &RenderMode) {
        if (MyRenderMode == RenderMode) {
            return;
        }

        if (IsActive()) {
            SetActive(false);
            MyRenderMode = RenderMode;
            SetActive(true);
        } else {
            MyRenderMode = RenderMode;
        }
    }

    // Sets the image of this renderer.
    void RenderComponent::SetImage(Image *Image) {
        if (MyImage == Image) {
            return;
        }

        if (IsActive()) {
            SetActive(false);
            MyImage = Image;
            SetActive(true);
        } else {
            MyImage = Image;
        }
    }

    // Returns the names of the target renderers of this renderer.
    void RenderComponent::SetTargetRenderers(const std::set<std::string> &TargetRenderers) {
        if (this->TargetRenderers == TargetRenderers) {
            return;
        }

        if (IsActive()) {
            SetActive(false);
            this->TargetRenderers = TargetRenderers;
            SetActive(true);
        } else {
            this->TargetRenderers = TargetRenderers;
        }
    }

    // Renders the renderer's texture first in the queue and therefore behind other textures.
    void RenderComponent::RenderFirst() {
        if (IsActive()) {
            SetActive(false);
            RenderingLast = false;
            SetActive(true);
        } else {
            RenderingLast = false;
        }
    }

    // Renders the renderer's texture last in the queue and therefore in front of other textures.
    void RenderComponent::RenderLast() {
        if (IsActive()) {
            SetActive(false);
            RenderingLast = true;
            SetActive(true);
        } else {
            RenderingLast = true;
        }
    }
}
