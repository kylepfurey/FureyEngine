// .h
// Engine Singleton Class Script
// by Kyle Furey

#pragma once
#include "../Renderer/Renderer.h"
#include "../Font/Font.h"
#include "../Audio/Audio.h"
#include "../Controller/Controller.h"
#include "../Animation/Animation.h"
#include "../Worlds/World.h"
#include "../Resource/Resource.h"

#define ENGINE_2D 1
#define ENGINE_3D 0

namespace FureyEngine {
    /** A singleton class used to store various reusable engine resources. */
    class Engine final {
        // ABSTRACT CLASS

        /** Prevents instantiation of this class. */
        virtual void Abstract() = 0;

    public:
        // RESOURCES

        /** Each public resource the engine stores. */
        class Resources final {
            // ABSTRACT CLASS

            /** Prevents instantiation of this class. */
            virtual void Abstract() = 0;

        public:
            // RESOURCES

            /**
             * Each window in the engine by name.<br/>
             * Use "main" to get the default window.
             */
            static Resource<Window> Windows;

            /**
             * Each shader in the engine by name.<br/>
             * Use "main" to get the default shader.
             */
            static Resource<Shader> Shaders;

            /**
             * Each renderer in the engine by name.<br/>
             * Use "main" to get the default renderer.
             */
            static Resource<Renderer> Renderers;

            /**
             * Each loaded image in the engine by name.<br/>
             * Instantiate your images here with their file name.
             */
            static Resource<Image> Images;

            /**
             * Each loaded animation in the engine by name.<br/>
             * Instantiate your animations here with their directory and count.
             */
            static Resource<Animation> Animations;

            /**
             * Each font file in the engine by name.<br/>
             * Instantiate your fonts here with their file name.
             */
            static Resource<Font> Fonts;

            /**
             * Each loaded sound in the engine by name.<br/>
             * Instantiate your sounds here with their file name.
             */
            static Resource<Audio> Audio;
        };

        // RENDERING

        /** Each renderer to render each tick. */
        static std::set<std::string> TargetRenderers;

        // CONTROLLERS

        /** A universal array of controllers. */
        static PointerArray<Controller> Controllers;

        // WORLDS

        /** Each loaded world by name, with each able to run custom scripting logic through its actors. */
        static std::map<std::string, std::shared_ptr<World> > Worlds;

        // CLEANUP

        /**
         * Events to call at the very end of an engine loop.<br/>
         * This event is cleared after each loop.
         */
        static Event Cleanup;

        /** Loads a new world of the given name. */
        template<typename WorldType, typename... ArgumentTypes>
        static Reference<World> LoadWorld(const std::string &WorldName, ArgumentTypes... Arguments) {
            const auto World = new WorldType(WorldName, Arguments...);
            Worlds[WorldName] = std::shared_ptr<WorldType>(World);
            return Worlds[WorldName];
        }

        /** Unloads the given world by name and returns if the given world was found. */
        static bool UnloadWorld(const std::string &UnloadedWorldName);

        /** Unloads the given world and returns if the given world was found. */
        static bool UnloadWorld(const Reference<World> &UnloadedWorld);

        // CLOSING

        /** Cleans up all resources before the engine closes. */
        static void Close();
    };

    /** Each public resource the engine stores. */
    using Resources = Engine::Resources;
}
