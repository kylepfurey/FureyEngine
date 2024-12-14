# FureyEngine
A simple class library used to streamline making 2D applications using OpenGL, GLEW, GLM, and SDL.


You can install all required packages by entering the following in a Windows command prompt:

git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
.\vcpkg install opengl glew glm sdl2 sdl2-image sdl2-ttf sdl2-mixer sdl2-net box2d


Development in FureyEngine is simple:

1. Inherit from the World class to create a world and spawn in actors.
2. Inherit from the Actor class to create and manage an object in your world, as well as add custom functionality by attaching components.
3. Inherit from the Component class to create modular code for your actors, or use some of the ones I have provided with the engine.
4. In main: Include FureyEngine.h, load your application's resources via FureyEngine::Resources, and load one of your new worlds in!
5. For more lower level control of the engine, feel free to modify any of the classes I have provided to tune the engine to your liking.
