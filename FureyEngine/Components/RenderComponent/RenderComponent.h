// .h
// Image Render Component Class Script
// by Kyle Furey

#pragma once
#include "../../Engine/Engine.h"

namespace FureyEngine {
    /**
     * A component used to render an image to the screen relative to the component's actor.<br/>
     * This component takes in a pointer to an image which is used to render that same image across multiple actors.
     */
    class RenderComponent : public Component {
    protected:
        // RENDERING

        /** The image used to generate a texture for this renderer. */
        Image *MyImage = nullptr;

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

    public:
        // CONSTRUCTORS

        /** Constructs a new render component for this actor. */
        explicit RenderComponent(Image *Image,
                                 const RenderMode &RenderMode = RenderMode::DYNAMIC_TEXTURE,
                                 const bool &RenderLast = true);

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

        /** Returns the image of this renderer. */
        [[nodiscard]] virtual Image *GetImage() const;

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

        /** Sets the image of this renderer. */
        virtual void SetImage(Image *Image);

        /** Sets the names of the target renderers of this renderer. */
        virtual void SetTargetRenderers(const std::set<std::string> &TargetRenderers);

        /** Renders the renderer's texture first in the queue and therefore behind other textures. */
        virtual void RenderFirst();

        /** Renders the renderer's texture last in the queue and therefore in front of other textures. */
        virtual void RenderLast();
    };
}
