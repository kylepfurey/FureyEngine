// .h
// Text Render Component Class Script
// by Kyle Furey

#pragma once
#include "../../Engine/Engine.h"

namespace FureyEngine {
    /**
     * A component used to render dynamic text from a font to the screen relative to the component's actor.<br/>
     * This component takes in a pointer to a font which is copied and managed to allow for custom text.
     */
    class TextComponent : public Component {
    protected:
        // FONT

        /** The custom font used to generate a text texture for this renderer. */
        Font *MyFont = nullptr;

        /**
         * The mode to render the generated texture with this renderer.<br/>
         * Dynamic  =   The texture will move with the actor.<br/>
         * Static   =   The texture will stay on the screen.
         */
        RenderMode MyRenderMode = RenderMode::DYNAMIC_TEXTURE;

        /** Whether to push this texture to the end of the queue when rendering. */
        bool RenderingLast = true;

        /**
         * Each of this render component's target renderers.<br/>
         * Each render component targets "main" by default.
         */
        std::set<std::string> TargetRenderers = {"main"};

        /** The generated texture of this renderer. */
        Texture MyTexture = {0, {0, 0}, nullptr};

        // FONT

        /**
         * Copies and safely initializes the given font.<br/>
         * Passing nullptr will destroy the font.
         */
        virtual void Reinitialize(const Font *Font,
                                  const std::string &Text = "",
                                  const int &Size = 32,
                                  const SDL_Color &Color = {255, 255, 255, 255});

    public:
        // CONSTRUCTORS

        /** Constructs a new text component for this actor. */
        explicit TextComponent(const Font *Font,
                               const std::string &Text,
                               const int &Size = 32,
                               const SDL_Color &Color = {255, 255, 255, 255},
                               const RenderMode &RenderMode = RenderMode::DYNAMIC_TEXTURE,
                               const bool &RenderLast = true);

        // DESTRUCTOR

        /** Destroys the font of this component. */
        ~TextComponent() override;

        // EVENTS

        /** Automatically called before each component has already called Start(). */
        void Attach() override;

        /** Automatically called after this component is removed. */
        void Remove() override;

        // ACTIVITY

        /**
         * Sets whether this component is active.<br/>
         * This can be overridden to control custom functionality.
         */
        void SetActive(const bool &Active) override;

        // GETTERS

        /** Returns the render mode of this renderer. */
        [[nodiscard]] virtual RenderMode GetRenderMode() const;

        /** Returns the font of this renderer. */
        [[nodiscard]] virtual Font *GetFont() const;

        /** Returns the current text of this component's font. */
        [[nodiscard]] virtual std::string GetText() const;

        /** Returns the current size of this component's size. */
        [[nodiscard]] virtual int GetSize() const;

        /** Returns the current color of this component's font. */
        [[nodiscard]] virtual SDL_Color GetColor() const;

        /** Returns the names of the target renderers of this renderer. */
        [[nodiscard]] virtual std::set<std::string> GetTargetRenderers() const;

        /** Returns the current generated texture of this renderer. */
        [[nodiscard]] virtual Texture GetTexture() const;

        /** Returns whether this texture was recently set to render first in the queue and therefore behind other textures. */
        [[nodiscard]] virtual bool IsRenderedFirst() const;

        /** Returns whether this texture was recently set to render last in the queue and therefore in front of other textures. */
        [[nodiscard]] virtual bool IsRenderedLast() const;

        // SETTERS

        /** Sets the render mode of this renderer. */
        virtual void SetRenderMode(const RenderMode &RenderMode);

        /** Sets the font of this renderer. */
        virtual void SetFont(Font *Font);

        /** Sets the current text of this component's font. */
        virtual void SetText(const std::string &Text);

        /** Sets the current size of this component's font. */
        virtual void SetSize(const int &Size);

        /** Sets the current color of this component's font. */
        virtual void SetColor(const SDL_Color &Color);

        /** Sets the names of the target renderers of this renderer. */
        virtual void SetTargetRenderers(const std::set<std::string> &TargetRenderers);

        /** Renders the renderer's texture first in the queue and therefore behind other textures. */
        virtual void RenderFirst();

        /** Renders the renderer's texture last in the queue and therefore in front of other textures. */
        virtual void RenderLast();
    };
}
