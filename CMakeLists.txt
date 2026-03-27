# ---------------------------------------------------
# Workspace CMake Setup
# ---------------------------------------------------
cmake_minimum_required(VERSION 3.8)

project(SphynxEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add custom configurations
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Shipping" CACHE STRING "" FORCE)

set(CMAKE_CXX_FLAGS_Debug "${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_SHARED_LINKER_FLAGS_Debug "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_Release "${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_SHARED_LINKER_FLAGS_Release "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_Shipping "${CMAKE_CXX_FLAGS_SHIPPING}")
set(CMAKE_SHARED_LINKER_FLAGS_Shipping "${CMAKE_SHARED_LINKER_FLAGS_SHIPPING}")

# Add subdirectories
add_subdirectory(vendor)
add_subdirectory(SphynxEngine)
add_subdirectory(SphynxEngine-Runtime)
add_subdirectory(SphynxEngine-Editor)
add_subdirectory(Sandbox)

# Hide CMake Generated Projects in a folder
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set StartUp project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sandbox)

set_target_properties(Sandbox PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_target_properties(SphynxEngine-Runtime PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_target_properties(SphynxEngine-Editor PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

function(enable_strict_warnings target_name)
    if (MSVC)
        target_compile_options(${target_name} PRIVATE
            /W4             # Nivel alto de warnings
            /WX             # Warnings como errores
            /w14242         # conversiones posibles pérdida de datos
            /w14254         # conversiones con signo
            /w14263         # conversiones entre tipos base/derivados
            /w14265         # conversiones entre punteros
            /w14287         # signed/unsigned mismatch
            /w14296         # cast C-style en expresiones
            /w14545 /w14546 # tamaño de buffer distinto
            /w14547 /w14549 # alineación y tamaño incorrectos
            /w14555         # operador condicional puede perder datos
            /w14640         # llamadas virtuales en constructores
            /w14826         # referencias nulas
            /w14905 /w14906 # tamaño de buffer distinto en CRT
        )
    else()
        target_compile_options(${target_name} PRIVATE
            -Wall
            -Wextra
            -Wconversion
            -Wsign-conversion
            -Wfloat-equal
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Wcast-align
            -Woverloaded-virtual
            -Wnull-dereference
            -Wdouble-promotion
            -Wformat=2
            -Werror
        )
    endif()
endfunction()

enable_strict_warnings(SphynxEngine)
enable_strict_warnings(SphynxEngine-Editor)
enable_strict_warnings(SphynxEngine-Runtime)
