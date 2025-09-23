# SphynxEngine

SphynxEngine is a **C++ game engine** project aimed at **cross-platform** development, with a strong focus on **modularity** and system independence. The architecture is built around the **Entity-Component-System (ECS)** model, enabling scalable and flexible game logic.

The engine integrates several external libraries such as **Box2D**, **SDL3**, and **EnTT**, all managed cleanly through **CMake** for portability and maintainability.

Aditionally, an **in-engine custom editor** interface is implemented using **Dear ImGui**, allowing real-time scene editing and debugging in a more visual and smoother way.

**✨ Core Features**

- 🖥 **Cross-Platform** – Designed with portability in mind to support multiple platforms. 
    - ⚠️ Right now, only **Windows** is supported.

- 🧩 **Modular Architecture** – Engine systems are independent and loosely coupled, promoting flexibility and maintainability.

- ⚙️ **CMake-Based Build System** – External dependencies (Box2D, SDL2, EnTT, etc.) are managed via CMake.

- 🧠 **Entity-Component-System (ECS)** – Built on EnTT, providing a clean and efficient data-oriented design.

- 🛠 **In-Engine Editor** – Custom built-in editor implemented with Dear ImGui for real-time scene manipulation and debugging.