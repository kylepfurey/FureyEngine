// .h
// Input Field Actor Class Script
// by Kyle Furey

#pragma once
#include "../ButtonActor/ButtonActor.h"

namespace FureyEngine {
    /** An actor with a set of components that enable it to be clicked on to enter and store text into. */
    class InputFieldActor : public ButtonActor {
    protected:
        // INPUT FIELD

        /** The currently selected input field. */
        static Reference<InputFieldActor> SelectedTextField;

        /** Normalizes the given string to allow it to fit within this input field. */
        virtual std::string NormalizeString(const std::string &String) const;

        /** A function used to handle updating this input field when text is input into the keyboard. */
        virtual void OnTextChanged(char Character, std::string String);

    public:
        // INPUT FIELD

        /**
         * An event called when the text is edited.<br/>
         * The only parameter is a string of the current text.
         */
        Delegate<std::string> OnEditText;

        /**
         * An event called when the input field is no longer selected.<br/>
         * The only parameter is a string of the current text.
         */
        Delegate<std::string> OnSubmit;

        /**
         * The maximum number of characters that can be entered into this input field.<br/>
         * A count of less than zero will be infinite.
         */
        int CharacterCount = -1;

        /** Whether numbers may only be entered into this input field. */
        bool NumbersOnly = false;

        /** Whether the spaces are allowed in this input field. */
        bool AllowSpaces = true;

        // CONSTRUCTORS

        /** Constructs a new input field with a name. */
        explicit InputFieldActor(const std::string &Name = "Field", const bool &Active = true);

        // EVENTS

        /** Automatically called after this actor is destroyed. */
        void Destroy() override;

        // INPUT FIELD

        /** Initializes this input field with the given parameters. */
        Reference<ButtonActor> Initialize(const Reference<CameraComponent> &Camera,
                                          Image *UnclickedImage,
                                          Image *HoveredImage = nullptr,
                                          Image *ClickedImage = nullptr,
                                          const Font *Font = nullptr,
                                          const std::string &Text = "",
                                          const int &Size = 32,
                                          const SDL_Color &Color = {255, 255, 255, 255}) override;

        /** Returns whether this input field is currently selected by the user. */
        [[nodiscard]] virtual bool IsSelected() const;

        /** Returns the current selected input field if there is one selected. k*/
        [[nodiscard]] static Reference<InputFieldActor> GetSelected();

        /** Marks this input field as the selected input field. */
        virtual void Select();

        /** Stops selecting the current selected input field and returns a reference to it if one was selected. */
        static Reference<InputFieldActor> Deselect();

        /** Returns whether the input text is empty. */
        [[nodiscard]] virtual bool IsEmpty() const;

        /** Returns the current character count of the input text. */
        [[nodiscard]] virtual int Length() const;

        /** Returns the current input text of this input field. */
        [[nodiscard]] virtual std::string GetText() const;

        /** Sets the given integer to the current input text of this input field as an integer, or returns false. */
        [[nodiscard]] virtual bool TryParseInt(int &Integer) const;

        /** Sets the given float to the current input text of this input field as a float, or returns false. */
        [[nodiscard]] virtual bool TryParseFloat(float &Float) const;

        /** Clears and returns the current input text. */
        virtual std::string Clear();

        /** Sets the current input text of this input field. */
        virtual void SetText(const std::string &Text) const;
    };
}
