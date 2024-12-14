// .cpp
// Text Render Component Class Script
// by Kyle Furey

#include "TextComponent.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Constructs a new text component for this actor.
    TextComponent::TextComponent(const Font *Font,
                                 const std::string &Text,
                                 const int &Size,
                                 const SDL_Color &Color,
                                 const RenderMode &RenderMode, const bool &RenderLast)
        : Component("Text", true), MyRenderMode(RenderMode), RenderingLast(RenderLast) {
        // Initialize the given font
        TextComponent::Reinitialize(Font, Text, Size, Color);

        MyTexture = Texture(0, {0, 0}, nullptr);
    }

    // DESTRUCTOR

    // Destroys the font of this component.
    TextComponent::~TextComponent() {
        // Deallocate this component's font
        TextComponent::Reinitialize(nullptr);
    }

    // FONT

    // Copies and safely initializes the given font.
    // Passing nullptr will destroy the font.
    void TextComponent::Reinitialize(const Font *Font, const std::string &Text, const int &Size,
                                     const SDL_Color &Color) {
        delete MyFont;
        if (Font != nullptr) {
            MyFont = new FureyEngine::Font(Font->Path(), Text, Size, Color);
        } else {
            MyFont = nullptr;
        }
    }

    // EVENTS

    // Automatically called before each component has already called Start().
    void TextComponent::Attach() {
        // Calls the base class's function
        Component::Attach();

        // Bind set active to the actor's set active function
        GetActor()->OnSetActive.Bind(std::bind(TextComponent::SetActive, this, std::placeholders::_1));

        // Set the texture's transform to this actor
        MyTexture.Transform = &GetActor()->Transform;

        // Calls SetActive() to initialize the texture
        SetActive(IsActive());
    }

    // Automatically called after this component is removed.
    void TextComponent::Remove() {
        // Calls the base class's function
        Component::Remove();

        // Unbind set active from the actor's set active function
        if (BeginTick) {
            GetActor()->OnSetActive.Unbind(std::bind(TextComponent::SetActive, this, std::placeholders::_1));
        }

        // Call SetActive() to clean up the texture
        SetActive(false);
    }

    // ACTIVITY

    // Sets whether this component is active.
    // This can be overridden to control custom functionality.
    void TextComponent::SetActive(const bool &Active) {
        // Calls the base class's function
        Component::SetActive(Active);

        if (Active) {
            if (MyTexture.ID == 0 && MyFont != nullptr) {
                MyTexture = {MyFont->TextureID(), MyFont->TextureSize(), MyTexture.Transform};
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
    RenderMode TextComponent::GetRenderMode() const {
        return MyRenderMode;
    }

    // Returns the font of this renderer.
    Font *TextComponent::GetFont() const {
        return MyFont;
    }

    // Returns the current text of this component's font.
    std::string TextComponent::GetText() const {
        return MyFont->Text();
    }

    // Returns the current size of this component's size.
    int TextComponent::GetSize() const {
        return MyFont->Size();
    }

    // Returns the current color of this component's font.
    SDL_Color TextComponent::GetColor() const {
        return MyFont->Color();
    }

    // Returns the names of the target renderers of this renderer.
    std::set<std::string> TextComponent::GetTargetRenderers() const {
        return TargetRenderers;
    }

    // Returns the current generated texture of this renderer.
    Texture TextComponent::GetTexture() const {
        return MyTexture;
    }

    // Returns whether this texture was recently set to render first in the queue and therefore behind other textures.
    bool TextComponent::IsRenderedFirst() const {
        return !RenderingLast;
    }

    // Returns whether this texture was recently set to render last in the queue and therefore in front of other textures.
    bool TextComponent::IsRenderedLast() const {
        return RenderingLast;
    }

    // SETTERS

    // Sets the render mode of this renderer.
    void TextComponent::SetRenderMode(const RenderMode &RenderMode) {
        if (IsActive()) {
            SetActive(false);
            MyRenderMode = RenderMode;
            SetActive(true);
        } else {
            MyRenderMode = RenderMode;
        }
    }

    // Sets the font of this renderer.
    void TextComponent::SetFont(Font *Font) {
        if (IsActive()) {
            SetActive(false);
            if (Font != nullptr) {
                Reinitialize(Font, MyFont->Text(), MyFont->Size(), MyFont->Color());
            } else {
                Reinitialize(Font);
            }
            SetActive(true);
        } else {
            if (Font != nullptr) {
                Reinitialize(Font, MyFont->Text(), MyFont->Size(), MyFont->Color());
            } else {
                Reinitialize(Font);
            }
        }
    }

    // Sets the current text of this component's font.
    void TextComponent::SetText(const std::string &Text) {
        if (MyFont != nullptr) {
            MyFont->Reinitialize(Text);
        }
    }

    // Sets the current size of this component's font.
    void TextComponent::SetSize(const int &Size) {
        if (MyFont != nullptr) {
            MyFont->Reinitialize(Size);
        }
    }

    // Sets the current color of this component's font.
    void TextComponent::SetColor(const SDL_Color &Color) {
        if (MyFont != nullptr) {
            MyFont->Reinitialize(Color);
        }
    }

    // Returns the names of the target renderers of this renderer.
    void TextComponent::SetTargetRenderers(const std::set<std::string> &TargetRenderers) {
        if (IsActive()) {
            SetActive(false);
            this->TargetRenderers = TargetRenderers;
            SetActive(true);
        } else {
            this->TargetRenderers = TargetRenderers;
        }
    }

    // Renders the renderer's texture first in the queue and therefore behind other textures.
    void TextComponent::RenderFirst() {
        if (IsActive()) {
            SetActive(false);
            RenderingLast = false;
            SetActive(true);
        } else {
            RenderingLast = false;
        }
    }

    // Renders the renderer's texture last in the queue and therefore in front of other textures.
    void TextComponent::RenderLast() {
        if (IsActive()) {
            SetActive(false);
            RenderingLast = true;
            SetActive(true);
        } else {
            RenderingLast = true;
        }
    }
}
