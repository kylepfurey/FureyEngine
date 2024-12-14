// .cpp
// Engine Resources Class Script
// by Kyle Furey

#include "Engine.h"

namespace FureyEngine {
    // WORLDS

    // Unloads the given world by name the next engine loop, and returns if the given world was found.
    bool Engine::UnloadWorld(const std::string &UnloadedWorldName) {
        if (Worlds.count(UnloadedWorldName)) {
            // Unload all of this world's actors
            for (const auto &Actor: Worlds[UnloadedWorldName]->Actors) {
                Worlds[UnloadedWorldName]->DestroyActor(Actor.get());
            }

            Cleanup.Bind([&] {
                if (Worlds[UnloadedWorldName] == nullptr) {
                    return;
                }

                // Call unload events
                Worlds[UnloadedWorldName]->Unload();
                Worlds[UnloadedWorldName]->OnUnload(Worlds[UnloadedWorldName].get());

                // Call cleanup events
                Worlds[UnloadedWorldName]->Cleanup();

                // Delete this world
                Worlds.erase(UnloadedWorldName);
            });
            return true;
        }
        return false;
    }

    // Unloads the given world and returns if the given world was found.
    bool Engine::UnloadWorld(const Reference<World> &UnloadedWorld) {
        return UnloadWorld(UnloadedWorld->Name);
    }

    // CLOSING

    // Cleans up all resources before the engine closes.
    void Engine::Close() {
        // Unload all worlds
        for (auto &[WorldName, World]: Worlds) {
            UnloadWorld(WorldName);
        }

        // Call cleanup events
        Cleanup();

        Controllers.Clear();
        TargetRenderers.clear();
        Resources::Audio.Clear();
        Resources::Fonts.Clear();
        Resources::Images.Clear();
        Resources::Animations.Clear();
        Resources::Renderers.Clear();
        Resources::Shaders.Clear();
        Resources::Windows.Clear();
    }

    // STATIC VARIABLE INITIALIZATION

    // Each window in the engine by name.
    // Use "main" to get the default window.
    Resource<Window> Engine::Resources::Windows;

    // Each shader in the engine by name.
    // Use "main" to get the default shader.
    Resource<Shader> Engine::Resources::Shaders;

    // Each renderer in the engine by name.
    // Use "main" to get the default renderer.
    Resource<Renderer> Engine::Resources::Renderers;

    // Each loaded image in the engine by name.
    // Instantiate your images here with their file name.
    Resource<Image> Engine::Resources::Images;

    // Each loaded animation in the engine by name.
    // Instantiate your animations here with their directory and count.
    Resource<Animation> Engine::Resources::Animations;

    // Each font file in the engine by name.
    // Instantiate your fonts here with their file name.
    Resource<Font> Engine::Resources::Fonts;

    // Each loaded sound in the engine by name.
    // Instantiate your sounds here with their file name.
    Resource<Audio> Engine::Resources::Audio;

    // Each renderer to render each tick.
    std::set<std::string> Engine::TargetRenderers = {"main"};

    // A universal array of controllers.
    PointerArray<Controller> Engine::Controllers;

    // Each world able to run custom scripting logic through its actors.
    std::map<std::string, std::shared_ptr<World> > Engine::Worlds;

    // Events to call at the very end of an engine loop.
    // This event is cleared after each loop.
    Event Engine::Cleanup;
}
