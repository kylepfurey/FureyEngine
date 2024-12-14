# CMAKE VERSION
cmake_minimum_required(VERSION 3.28)
project(FureyEngine)

# C++ VERSION
set(CMAKE_CXX_STANDARD 17)

# SET PROJECT ROOT DIRECTORY
add_definitions(-DROOT="${CMAKE_SOURCE_DIR}/")

# ENTER THE FOLLOWING IN THE COMMAND PROMPT (WINDOWS)
# git clone https://github.com/microsoft/vcpkg.git
# cd vcpkg
# .\bootstrap-vcpkg.bat
# .\vcpkg integrate install
# .\vcpkg install opengl glew glm sdl2 sdl2-image sdl2-ttf sdl2-mixer sdl2-net box2d

# UPDATE THIS WITH YOUR USERNAME
set(USER "${kylef}")

# SET TOOLCHAIN
set(CMAKE_TOOLCHAIN_FILE "C:/Users/${USER}/vcpkg/scripts/buildsystems/vcpkg.cmake")

# FIND PACKAGES
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(SDL2_net CONFIG REQUIRED)
find_package(box2d CONFIG REQUIRED)

# FUREY ENGINE EXECUTABLES
add_executable(FureyEngine
        main.cpp
        FureyEngine/FureyEngine.h
        FureyEngine/Standard/Standard.h
        FureyEngine/Window/Window.cpp
        FureyEngine/Window/Window.h
        FureyEngine/Shader/Shader.cpp
        FureyEngine/Shader/Shader.h
        FureyEngine/Quad/Quad.h
        FureyEngine/Transform/Transform.cpp
        FureyEngine/Transform/Transform.h
        FureyEngine/Math/Math.h
        FureyEngine/Texture/Texture.cpp
        FureyEngine/Texture/Texture.h
        FureyEngine/Renderer/Renderer.cpp
        FureyEngine/Renderer/Renderer.h
        FureyEngine/Image/Image.cpp
        FureyEngine/Image/Image.h
        FureyEngine/Font/Font.cpp
        FureyEngine/Font/Font.h
        FureyEngine/Audio/Audio.cpp
        FureyEngine/Audio/Audio.h
        FureyEngine/Delegate/Delegate.h
        FureyEngine/Event/Event.cpp
        FureyEngine/Event/Event.h
        FureyEngine/Input/Input.cpp
        FureyEngine/Input/Input.h
        FureyEngine/Controller/Controller.cpp
        FureyEngine/Controller/Controller.h
        FureyEngine/Thread/Thread.cpp
        FureyEngine/Thread/Thread.h
        FureyEngine/PointerArray/PointerArray.h
        FureyEngine/Animation/Animation.cpp
        FureyEngine/Animation/Animation.h
        FureyEngine/Reference/Reference.h
        FureyEngine/Components/Component.cpp
        FureyEngine/Components/Component.h
        FureyEngine/Actors/Actor.cpp
        FureyEngine/Actors/Actor.h
        FureyEngine/Worlds/World.cpp
        FureyEngine/Worlds/World.h
        FureyEngine/Resource/Resource.h
        FureyEngine/Engine/Engine.cpp
        FureyEngine/Engine/Engine.h
        FureyEngine/Components/RenderComponent/RenderComponent.cpp
        FureyEngine/Components/RenderComponent/RenderComponent.h
        FureyEngine/Components/TextComponent/TextComponent.cpp
        FureyEngine/Components/TextComponent/TextComponent.h
        FureyEngine/Components/AnimationComponent/AnimationComponent.cpp
        FureyEngine/Components/AnimationComponent/AnimationComponent.h
        FureyEngine/Components/CameraComponent/CameraComponent.cpp
        FureyEngine/Components/CameraComponent/CameraComponent.h
        FureyEngine/Components/ChildActorComponent/ChildActorComponent.cpp
        FureyEngine/Components/ChildActorComponent/ChildActorComponent.h
        FureyEngine/Components/BrushComponent/BrushComponent.cpp
        FureyEngine/Components/BrushComponent/BrushComponent.h
        FureyEngine/Components/MovementComponent/MovementComponent.cpp
        FureyEngine/Components/MovementComponent/MovementComponent.h
        FureyEngine/Components/PhysicsComponent/PhysicsComponent.cpp
        FureyEngine/Components/PhysicsComponent/PhysicsComponent.h
        FureyEngine/Components/StateMachineComponent/StateMachineComponent.cpp
        FureyEngine/Components/StateMachineComponent/StateMachineComponent.h
        FureyEngine/Actors/PawnActor/PawnActor.cpp
        FureyEngine/Actors/PawnActor/PawnActor.h
        FureyEngine/Actors/SpriteActor/SpriteActor.cpp
        FureyEngine/Actors/SpriteActor/SpriteActor.h
        FureyEngine/Actors/TextActor/TextActor.cpp
        FureyEngine/Actors/TextActor/TextActor.h
        FureyEngine/Actors/ColliderActor/ColliderActor.cpp
        FureyEngine/Actors/ColliderActor/ColliderActor.h
        FureyEngine/Actors/TriggerActor/TriggerActor.cpp
        FureyEngine/Actors/TriggerActor/TriggerActor.h
        FureyEngine/Actors/TriggerActor/TriggerActor.cpp
        FureyEngine/Actors/TriggerActor/TriggerActor.h
        FureyEngine/Actors/ColliderActor/ColliderActor.cpp
        FureyEngine/Actors/ColliderActor/ColliderActor.h
        FureyEngine/Actors/ButtonActor/ButtonActor.cpp
        FureyEngine/Actors/ButtonActor/ButtonActor.h
        FureyEngine/Actors/InputFieldActor/InputFieldActor.cpp
        FureyEngine/Actors/InputFieldActor/InputFieldActor.h

        # UPDATE THIS WITH YOUR CUSTOM CODE
)

# LINK ALL LIBRARIES
target_link_libraries(FureyEngine PRIVATE
        OpenGL::GL
        GLEW::GLEW
        glm::glm
        SDL2::SDL2
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf
        SDL2_mixer::SDL2_mixer
        SDL2_net::SDL2_net
        box2d::box2d
)
