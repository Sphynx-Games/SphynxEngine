# ---------------------------------------------------
# Workspace CMake Setup
# ---------------------------------------------------
cmake_minimum_required(VERSION 3.8)

project(SphynxEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COPILER "msvc")

add_subdirectory(SphynxEngine)
add_subdirectory(SphynxEngineEditor)
add_subdirectory(Sandbox)
add_subdirectory(vendor)

# Hide CMake Generated Projects in a folder
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set StartUp project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sandbox)

if (WIN32)
	target_compile_definitions(SphynxEngine PUBLIC SPX_PLATFORM_WINDOWS)
	target_compile_definitions(Sandbox PUBLIC SPX_PLATFORM_WINDOWS)
endif()

set_target_properties(Sandbox PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_target_properties(SphynxEngineEditor PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")