// .h
// Text Actor Class Script
// by Kyle Furey

#pragma once
#include "../../Components/TextComponent/TextComponent.h"

namespace FureyEngine {
    /** An actor with a text component used to easily display dynamic text. */
    class TextActor : public Actor {
    protected:
        // TEXT

        /** Whether this sprite has been initialized yet. */
        bool Initialized = false;

    public:
        // TEXT

        /**
         * A reference to the text's component, used to render text to the screen.<br/>
         * This is automatically attached when Initialize() is called.
         */
        Reference<TextComponent> TextComponent = nullref;

        // CONSTRUCTORS

        /** Constructs new text with a name. */
        explicit TextActor(const std::string &Name = "Text", const bool &Active = true);

        // TEXT

        /** Returns whether this text has been initialized yet. */
        [[nodiscard]] virtual bool IsInitialized() const;

        /** Initializes this text's component with the given font parameters. */
        virtual Reference<TextActor> Initialize(const Font *Font,
                                                const std::string &Text = "",
                                                const int &Size = 32,
                                                const SDL_Color &Color = {255, 255, 255, 255});
    };
}
