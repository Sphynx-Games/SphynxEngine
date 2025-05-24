# ---------------------------------------------------
# Workspace CMake Setup
# ---------------------------------------------------
cmake_minimum_required(VERSION 3.8)

project(SphynxEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(vendor)
add_subdirectory(SphynxEngine)
add_subdirectory(SphynxEngine-Runtime)
add_subdirectory(SphynxEngine-Editor)
add_subdirectory(Sandbox)

# Hide CMake Generated Projects in a folder
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set StartUp project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SphynxEngine-Runtime)

set_target_properties(Sandbox PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_target_properties(SphynxEngine-Runtime PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_target_properties(SphynxEngine-Editor PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")