// .cpp
// Text Actor Class Script
// by Kyle Furey

#include "TextActor.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Constructs new text with a name.
    TextActor::TextActor(const std::string &Name, const bool &Active)
        : Actor(Name, Active) {
    }

    // TEXT

    // Returns whether this text has been initialized yet.
    bool TextActor::IsInitialized() const {
        return Initialized;
    }

    // Initializes this text's component with the given font parameters.
    Reference<TextActor> TextActor::Initialize(const Font *Font,
                                               const std::string &Text,
                                               const int &Size,
                                               const SDL_Color &Color) {
        if (!Initialized) {
            Initialized = true;
            TextComponent = AttachComponent<FureyEngine::TextComponent>(Font, Text, Size, Color);
        }
        return {this};
    }
}
