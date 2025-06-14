#pragma once

// Include all client headers
#include "Core/Application.h"
#include "Core/Layer.h"
#include "Core/UUID.h"

// Logging
#include "Logging/Log.h"
#include "Logging/LogDefines.h"

// Input
#include "Input/Input.h"
#include "Input/Keycode.h"
#include "Input/MouseButton.h"

// Command
#include "Command/CommandManager.h"

// Time
#include "Core/Time.h"

// Containers
#include "Container/Set.h"
#include "Container/Array.h"
#include "Container/Map.h"
#include "Container/Pair.h"

// Rendering
#include "Renderer/Window.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/Texture.h"
#include "Renderer/Sprite.h"
#include "Renderer/Font.h"

// Scene
#include "Scene/Scene.h"
#include "Scene/Actor.h"

// Components
#include "Component/Components.h"

// Serialization
#include "Serialization/FileReader.h"
#include "Serialization/FileWriter.h"
#include "Serialization/Scene/SceneDeserializer.h"
#include "Serialization/Scene/SceneSerializer.h"

// Assets
#include "Asset/AssetManager.h"
#include "Asset/Texture/TextureAsset.h"
#include "Asset/Sprite/SpriteAsset.h"
#include "Asset/Spritesheet/SpritesheetAsset.h"
#include "Asset/Font/FontAsset.h"