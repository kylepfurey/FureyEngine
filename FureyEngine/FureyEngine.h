// .h
// Furey Engine Script
// by Kyle Furey

#pragma once

// FUREY ENGINE

#define PROJECT_NAME "Furey Engine"
#define PROJECT_CREATOR "Kyle Furey"
#define DEBUG 1

#define SDL_MAIN_HANDLED 1
#include "Standard/Standard.h"
#include "Window/Window.h"
#include "Shader/Shader.h"
#include "Quad/Quad.h"
#include "Transform/Transform.h"
#include "Math/Math.h"
#include "Texture/Texture.h"
#include "Renderer/Renderer.h"
#include "Image/Image.h"
#include "Font/Font.h"
#include "Audio/Audio.h"
#include "Delegate/Delegate.h"
#include "Event/Event.h"
#include "Input/Input.h"
#include "Controller/Controller.h"
#include "Thread/Thread.h"
#include "PointerArray/PointerArray.h"
#include "Animation/Animation.h"
#include "Reference/Reference.h"
#include "Components/Component.h"
#include "Actors/Actor.h"
#include "Worlds/World.h"
#include "Resource/Resource.h"
#include "Engine/Engine.h"
#include "Components/RenderComponent/RenderComponent.h"
#include "Components/TextComponent/TextComponent.h"
#include "Components/AnimationComponent/AnimationComponent.h"
#include "Components/CameraComponent/CameraComponent.h"
#include "Components/ChildActorComponent/ChildActorComponent.h"
#include "Components/BrushComponent/BrushComponent.h"
#include "Components/MovementComponent/MovementComponent.h"
#include "Components/PhysicsComponent/PhysicsComponent.h"
#include "Components/StateMachineComponent/StateMachineComponent.h"
#include "Actors/PawnActor/PawnActor.h"
#include "Actors/SpriteActor/SpriteActor.h"
#include "Actors/TextActor/TextActor.h"
#include "Actors/ColliderActor/ColliderActor.h"
#include "Actors/TriggerActor/TriggerActor.h"
#include "Actors/ButtonActor/ButtonActor.h"
#include "Actors/InputFieldActor/InputFieldActor.h"

// MAIN

/** Furey Engine in its entirety. */
namespace FureyEngine {
    /** The entry point of the Furey Engine. */
    int Main(int &argc, char **&argv);
}

/**
* The main function used to initialize Furey Engine.<br/>
* This is always called before the user defined main function.
*/
int main(int argc, char **argv) {
#if DEBUG
    try {
#endif
        // Create a new window
        FureyEngine::Resources::Windows("main",
                                        PROJECT_NAME "  -  " PROJECT_CREATOR,
                                        TARGET_RESOLUTION_WIDTH,
                                        TARGET_RESOLUTION_HEIGHT);

        // Create a shader
        FureyEngine::Resources::Shaders("main",
                                        ROOT"FureyEngine/Shader/VertexShader.glsl",
                                        ROOT"FureyEngine/Shader/FragmentShader.glsl");

        // Create a renderer
        FureyEngine::Resources::Renderers("main",
                                          &FureyEngine::Resources::Shaders["main"],
                                          &FureyEngine::Resources::Windows["main"]);

        // Bind quit events
        FureyEngine::Input::KeyboardEvents[SDLK_ESCAPE].Bind([&](auto) { FureyEngine::Input::Quit(); });

        // Run Furey Engine
        const int Code = FureyEngine::Main(argc, argv);

        // Run game logic if configuration of Furey Engine was successful
        if (Code == 0) {
            // Run the game loop
            bool Running = true;
            while (Running) {
#if DEBUG
                // Check for errors
                GL_ERROR_CHECK;
                SDL_ERROR_CHECK;
#endif

                // Update input events
                FureyEngine::Input::Update(Running);
                if (!Running) {
                    break;
                }

                // Update the current world(s)
                for (const auto &[WorldName, World]: FureyEngine::Engine::Worlds) {
                    World->Update();
                }

                // Clear the current window(s)
                for (const auto &RendererName: FureyEngine::Engine::TargetRenderers) {
                    FureyEngine::Resources::Renderers[RendererName].Clear();
                }

                // Render the current window(s)
                for (const auto &RendererName: FureyEngine::Engine::TargetRenderers) {
                    FureyEngine::Resources::Renderers[RendererName].Render();
                }

                // Call cleanup events
                FureyEngine::Engine::Cleanup();
                FureyEngine::Engine::Cleanup.Clear();
            }
        }

        // Clean up all engine resources
        FureyEngine::Engine::Close();

        return Code;
#if DEBUG
    } catch (const std::exception &Exception) {
        std::cerr << Exception.what() << std::endl;
        std::abort();
    }
#endif
}

// The entry point of the Furey Engine.
#define main(...) FureyEngine::Main(int &argc, char **&argv)
