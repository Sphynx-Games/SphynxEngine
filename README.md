# SphynxEngine

SphynxEngine is a **C++ game engine** project aimed at **cross-platform** development, with a strong focus on **modularity** and system independence. The architecture is built around the **Entity-Component-System (ECS)** model, enabling scalable and flexible game logic.

The engine integrates several external libraries such as **Box2D**, **SDL3**, and **EnTT**, all managed cleanly through **CMake** for portability and maintainability.

Aditionally, an **in-engine custom editor** interface is implemented using **Dear ImGui**, allowing real-time scene editing and debugging in a more visual and smoother way.

### **✨ Core Features**

- 🖥 **Cross-Platform** – Designed with portability in mind to support multiple platforms. 
    - ⚠️ Right now, only **Windows** is supported.

- 🧩 **Modular Architecture** – Engine systems are independent and loosely coupled, promoting flexibility and maintainability.

- ⚙️ **CMake-Based Build System** – External dependencies (Box2D, SDL2, EnTT, etc.) are managed via CMake.

- 🧠 **Entity-Component-System (ECS)** – Built on EnTT, providing a clean and efficient data-oriented design.

- 🎨 **In-Engine Editor** – Custom built-in editor implemented with Dear ImGui for real-time scene manipulation and debugging.

## **📂 Project Structure**

The repository is organized into 4 main projects:
- **🛠️ SphynxEngine** - The engine itself.
- **🎨 SphynxEngine-Editor** - The in-game editor.
- **⚡ SphynxEngine-Runtime** - Runtime executable for engine.
- **🧪 Sandbox** - Test project to experiment with the engine.

SphynxEngine and Sandbox are both compiled as DLLs that can be used by the runtime and the editor.

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