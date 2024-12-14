// .cpp
// Input Field Actor Class Script
// by Kyle Furey

#include "InputFieldActor.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Constructs a new input field with a name.
    InputFieldActor::InputFieldActor(const std::string &Name, const bool &Active)
        : ButtonActor(Name, Active) {
    }

    // EVENTS

    // Automatically called after this actor is destroyed.
    void InputFieldActor::Destroy() {
        // Calls the base class's function
        ButtonActor::Destroy();

        if (IsSelected()) {
            Deselect();
        }
    }

    // INPUT FIELD

    // Normalizes the given string to allow it to fit within this input field.
    std::string InputFieldActor::NormalizeString(const std::string &String) const {
        std::string NewString;
        const int Count = CharacterCount < 0 || CharacterCount >= String.length()
                              ? static_cast<int>(String.length())
                              : CharacterCount;

        for (int i = 0; i < Count; ++i) {
            switch (String[i]) {
                default:
                    if (!NumbersOnly) {
                        NewString += String[i];
                    }
                    break;
                case '\n':
                    break;
                case '\t':
                    break;
                case '\r':
                    break;
                case ' ':
                    if (AllowSpaces) {
                        NewString += String[i];
                    }
                    break;
                case '0':
                    NewString += String[i];
                    break;
                case '1':
                    NewString += String[i];
                    break;
                case '2':
                    NewString += String[i];
                    break;
                case '3':
                    NewString += String[i];
                    break;
                case '4':
                    NewString += String[i];
                    break;
                case '5':
                    NewString += String[i];
                    break;
                case '6':
                    NewString += String[i];
                    break;
                case '7':
                    NewString += String[i];
                    break;
                case '8':
                    NewString += String[i];
                    break;
                case '9':
                    NewString += String[i];
                    break;
            }
        }

        return NewString;
    }

    // A function used to handle updating this input field when text is input into the keyboard.
    void InputFieldActor::OnTextChanged(const char Character, std::string String) {
        if (TextComponent) {
            String = TextComponent->GetText();
            if (CharacterCount < 0 || String.length() < CharacterCount) {
                if (const std::string NewString = NormalizeString(String + Character); String != NewString) {
                    TextComponent->SetText(NewString);
                    OnEditText(NewString);
                }
            }
        }
    }

    // Initializes this input field with the given parameters.
    Reference<ButtonActor> InputFieldActor::Initialize(const Reference<CameraComponent> &Camera,
                                                       Image *UnclickedImage,
                                                       Image *HoveredImage,
                                                       Image *ClickedImage,
                                                       const Font *Font,
                                                       const std::string &Text,
                                                       const int &Size,
                                                       const SDL_Color &Color) {
        if (!Initialized) {
            // Calls the base class's function
            const auto Me = ButtonActor::Initialize(Camera,
                                                    UnclickedImage, HoveredImage, ClickedImage,
                                                    Font, Text, Size, Color);

            Toggle = true;
            ClickOnRelease = false;
            OnClick.Bind([this] {
                Clicked = true;
                this->Select();
            });
            OnUnclick.Bind([this] {
                Clicked = true;
                this->Select();
            });
            TextComponent->SetText(NormalizeString(Text));

            return Me;
        }

        // Calls the base class's function
        return ButtonActor::Initialize(Camera,
                                       UnclickedImage, HoveredImage, ClickedImage,
                                       Font, Text, Size, Color);
    }

    // Returns whether this input field is currently selected by the user.
    bool InputFieldActor::IsSelected() const {
        return SelectedTextField && static_cast<InputFieldActor *>(SelectedTextField) == this;
    }

    // Returns the current selected input field if there is one selected.
    Reference<InputFieldActor> InputFieldActor::GetSelected() {
        return SelectedTextField;
    }

    // Marks this input field as the selected input field.
    void InputFieldActor::Select() {
        if (IsSelected()) {
            return;
        }

        Deselect();

        if (Clickable) {
            SelectedTextField = this;

            Input::ClearText();
            Input::StartText();
            Input::TextEvents.Bind([this](const char &Character, const std::string &String) {
                OnTextChanged(Character, String);
            });
            Input::MouseButtonEvents[LEFT_MOUSE_BUTTON].Bind([this](auto, const ButtonState &State) {
                if (State == ButtonState::DOWN && IsSelected() && !IsHovered()) {
                    Deselect();
                }
            });
            Input::KeyboardEvents[SDLK_RETURN].Bind([this](const ButtonState &State) {
                if (State == ButtonState::DOWN && IsSelected()) {
                    Deselect();
                }
            });
            /*Input::KeyboardEvents[SDLK_ESCAPE].Bind([this](const ButtonState &State) {
                if (State == ButtonState::DOWN && IsSelected()) {
                    Deselect();
                }
            });*/
            Input::KeyboardEvents[SDLK_BACKSPACE].Bind([this](const ButtonState &State) {
                if (State == ButtonState::DOWN && TextComponent && IsSelected()) {
                    if (std::string MyText = GetText(); !MyText.empty()) {
                        TextComponent->SetText(MyText.erase(MyText.length() - 1, 1));
                        OnEditText(TextComponent->GetText());
                    }
                }
            });
        }
    }

    // Stops selecting the current selected input field and returns a reference to it if one was selected.
    Reference<InputFieldActor> InputFieldActor::Deselect() {
        const auto Previous = SelectedTextField;
        if (Previous) {
            if (Previous->IsClicked()) {
                Previous->Click();
                Previous->Clicked = false;
            }

            Previous->OnSubmit(Previous->GetText());

            Input::StopText();
            Input::TextEvents.Unbind([Previous](const char &Character, const std::string &String) {
            });
            Input::MouseButtonEvents[LEFT_MOUSE_BUTTON].Unbind([Previous](auto, const ButtonState &State) {
            });
            Input::KeyboardEvents[SDLK_RETURN].Unbind([Previous](const ButtonState &State) {
            });
            /*Input::KeyboardEvents[SDLK_ESCAPE].Unbind([Previous](const ButtonState &State) {
            });*/
            Input::KeyboardEvents[SDLK_BACKSPACE].Unbind([Previous](const ButtonState &State) {
            });
        }

        SelectedTextField = nullref;

        return Previous;
    }

    // Returns whether the input text is empty.
    bool InputFieldActor::IsEmpty() const {
        return !TextComponent || TextComponent->GetText().empty();
    }

    // Returns the current character count of the input text.
    int InputFieldActor::Length() const {
        if (!TextComponent) {
            return 0;
        }
        return static_cast<int>(TextComponent->GetText().length());
    }

    // Returns the current input text of this input field.
    std::string InputFieldActor::GetText() const {
        if (!TextComponent) {
            return "";
        }
        return TextComponent->GetText();
    }

    // Sets the given integer to the current input text of this input field as an integer, or returns false.
    bool InputFieldActor::TryParseInt(int &Integer) const {
        if (!TextComponent) {
            return false;
        }

        if (std::stringstream StringStream(TextComponent->GetText()); StringStream >> Integer) {
            return true;
        }

        return false;
    }

    // Sets the given float to the current input text of this input field as a float, or returns false.
    bool InputFieldActor::TryParseFloat(float &Float) const {
        if (!TextComponent) {
            return false;
        }

        if (std::stringstream StringStream(TextComponent->GetText()); StringStream >> Float) {
            return true;
        }

        return false;
    }

    // Clears and returns the current input text.
    std::string InputFieldActor::Clear() {
        if (!TextComponent) {
            return "";
        }
        const std::string String = TextComponent->GetText();
        TextComponent->SetText("");
        return String;
    }

    // Sets the current input text of this input field.
    void InputFieldActor::SetText(const std::string &Text) const {
        if (!TextComponent) {
            return;
        }
        TextComponent->SetText(NormalizeString(Text));
    }

    // STATIC VARIABLE INITIALIZATION

    // The currently selected input field.
    Reference<InputFieldActor> InputFieldActor::SelectedTextField = nullref;
}
