// .h
// Template Component Class Script
// by Kyle Furey

#pragma once
#include "../../Engine/Engine.h"

namespace FureyEngine {
    /** An example component script that can be copied to create new components. */
    class ExampleComponent : public Component {
    protected:
        // VARIABLES

        // New variables here

    public:
        // CONSTRUCTORS

        /** Constructs a new component with a name. */
        explicit ExampleComponent(const std::string &Name = "New Component", const bool &Active = true);

        // New constructors here

        // EVENTS

        /** Automatically called before each component has already called Start(). */
        void Attach() override;

        /** Automatically called when this component is first created. */
        void Start() override;

        /** Automatically called each world tick. */
        void Tick(const double &DeltaTime) override;

        /** Automatically called after each component has already called Tick(). */
        void LateTick(const double &DeltaTime) override;

        /** Automatically called after this component is removed. */
        void Remove() override;

        // ACTIVITY

        /**
         * Sets whether this component is active.<br/>
         * This can be overridden to control custom functionality.
         */
        void SetActive(const bool &Active) override;

        // FUNCTIONS

        // New functions here
    };
}
