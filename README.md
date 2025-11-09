# SphynxEngine

> ### ***⚠️ Disclaimer***: *This project is intended for **learning purposes only**.*
> This project must not be used for developing commercial games without acquiring the proper licenses of the libraries used as dependencies.
***

SphynxEngine is a **C++ game engine** project aimed at **cross-platform** development, with a strong focus on **modularity** and system independence. The architecture is built around the **Entity-Component-System (ECS)** model, enabling scalable and flexible game logic.

The engine integrates several external libraries such as **Box2D**, **SDL3**, and **EnTT**, all managed cleanly through **CMake** for portability and maintainability.

Aditionally, an **in-engine custom editor** interface is implemented using **Dear ImGui**, allowing real-time scene editing and debugging in a more visual and smoother way.


### **✨ Core Features**

- 🖥 **Cross-Platform** – Designed with portability in mind to support multiple platforms, even though, right now, only **Windows** is supported.

- 🧩 **Modular Architecture** – Engine systems are independent and loosely coupled, promoting flexibility and maintainability.

- ⚙️ **CMake-Based Build System** – External dependencies (Box2D, SDL3, EnTT, etc.) are managed via CMake.

- 🧠 **Entity-Component-System (ECS)** – Built on EnTT, providing a clean and efficient data-oriented design.

- 🎨 **In-Engine Editor** – Custom built-in editor implemented with Dear ImGui for real-time scene manipulation and debugging.


## **📂 Project Structure**

The repository is organized into 4 main projects:
- **🛠️ SphynxEngine** - The engine itself.
- **🎨 SphynxEngine-Editor** - The in-game editor.
- **⚡ SphynxEngine-Runtime** - Runtime executable for engine.
- **🧪 Sandbox** - Test project to experiment with the engine.

SphynxEngine and Sandbox are both compiled as DLLs that can be used by the Runtime and the Editor.


## **⚙️ Install, Build and Run**

To **clone the repository** with all submodules, execute:

    git clone --recursive https://github.com/Sphynx-Games/SphynxEngine.git
    cd SphynxEngine

To **create the Visual Studio solution**, simply run:

    ./generate.bat

This will execute CMake, download dependencies, and configure the solution ready for building.

Once the solution is generated, there are several options:

1. **Build and run the Editor or Runtime**

    - Select the Editor or Runtime as the startup project.
    - Build with a preferred configuration.

2. **Using the Sandbox**

    Having Sandbox as the startup project automatically launches the right module depending on the selected configuration.

    | 🔧 Config | 🚀 Launches | 📝 Description |
    |------------------|------------|----------------|
    | **Debug**        | 🎨 Editor  | Launches the Editor with the Sandbox project (debug mode). |
    | **Release**      | 🎨 Editor  | Launches the editor with the Sandbox project (optimized build). |
    | **Shipping**     | ⚡ Runtime | Launches the runtime with the Sandbox project (final distribution mode). |


## **🧩 Engine Systems**

The engine contains multiple systems:

- **🖌️ Rendering System**

    Handles the rendering pipeline, including window creation, 2D drawing and graphics API abstraction. Internally, the system is using **SDL3** and **OpenGL** libraries to execute the render operations. The graphics API abstraction allows to change the current graphic module at runtime.

- **⚛️ Physics System**

    Provides collision detection, rigid body dynamics, 2D physics simulation and physics API abstraction. The system uses **Box2D** internally, but it allows changing between physics modules at compile time.

- **🎵 Audio System**
    
    Manages sound effects and music playback, and provides sound API abstraction. The system uses **FMOD** internally, but it allows changing between audio modules at compile time.

- **🎮 Event System**

    Processes input (keyboard, mouse and gamepad) and window events. Provides event handling and input mapping for gameplay and editor tools.

- **🌍 Entity Component System**
    
    Responsible for managing entities (actors), components and scenes thanks to the **EnTT** library.

- **🔮 Reflection System**

    Enables runtime type information and introspection. Supports dynamic discovery of components, properties, and functions, allowing flexible scripting and editor integration.

- **🗂️ Asset Management System**

    Manages saving and loading assets to/from disk. Leverages the Reflection System to automatically serialize object properties and reconstruct them at runtime. The currently supported assets are:
    - Textures
    - Fonts
    - Sprites and Spritesheets
    - Animations
    - Sounds
    - Scenes
    - Prefabs

- **🎞️ Animation System**

    Handles sprite animations.

- **📝 Logging System**

    Handles engine messages, warnings, and errors.

- **📝 Module Management System**

    Responsible for loading and unloading modules based on the target platform.


## **📌 Roadmap**

Planned features and improvements for future versions of the engine:

- 🎨 Improve the Editor UI
- 🖼️ Add Game UI
- 📦 Add packaging/export system for final game builds
- 🖌️ Add 3D Rendering
- 🎞️ Add 3D Animation
- 🖥 Add support to other operating systems (e.g. Linux)