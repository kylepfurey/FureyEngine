// .cpp
// Furey Engine Entry Point
// by Kyle Furey

// Include this heading to use the Furey Engine.
#include "FureyEngine/FureyEngine.h"

using namespace std;
using namespace glm;
using namespace FureyEngine;

/** The entry point of the Furey Engine. */
int main(int argc, char **argv) {
    // Load your resources
    Resources::Images("Image", ROOT"Resources/Image.png");
    Resources::Fonts("Font", ROOT"Resources/Font.ttf");
    Resources::Audio("Audio", ROOT"Resources/Audio.wav");

    // Load a world
    const auto World =
            Engine::LoadWorld<::World>("New World");

    // Spawn a player
    const auto Player =
            World->SpawnActor<PawnActor>(Transform())
            ->Initialize(&Resources::Images["Image"]);

    // Spawn an obstacle
    const auto Obstacle =
            World->SpawnActor<SpriteActor>(Transform(Player->CameraComponent->TopRight(), 0, {4, 2}))
            ->Initialize(&Resources::Images["Image"], true, true);
    Obstacle->PhysicsComponent->UseGravity = false;
    Obstacle->PhysicsComponent->AngularVelocity = 100;

    // Play a sound
    Resources::Audio["Audio"].Play();

    return 0;
}
