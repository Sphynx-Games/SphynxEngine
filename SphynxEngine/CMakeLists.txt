# ---------------------------------------------------
# Sphynx Engine Project CMake Setup
# ---------------------------------------------------
cmake_minimum_required(VERSION 3.6)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
else()
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif()

project(SphynxEngine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin-int/${PROJECT_NAME}/$<CONFIG>/)
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin-int/${PROJECT_NAME}/$<CONFIG>/)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}/$<CONFIG>/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}/$<CONFIG>/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}/$<CONFIG>/)

option(SPHYNX_BUILD_SHARED "Build as a dynamic library" ON)

# Get all my .cpp files and create variable SPXSOURCES 
file(GLOB_RECURSE SPXSOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp" 
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" )

# Set the library name and the sources to be compiled
if(SPHYNX_BUILD_SHARED)
	add_library(${PROJECT_NAME} SHARED ${SPXSOURCES})
else()
	add_library(${PROJECT_NAME} STATIC ${SPXSOURCES})
endif()

# Set target sources
target_sources(${PROJECT_NAME} PRIVATE ${SPXSOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/")
target_link_libraries(${PROJECT_NAME} PUBLIC vendor)

# Set Precompiled Header
target_precompile_headers(${PROJECT_NAME} PRIVATE "src/spxpch.h")

target_compile_definitions(${PROJECT_NAME} PUBLIC SPX_PHYSICS_2D_BOX2D)

target_compile_definitions(${PROJECT_NAME}  PUBLIC
	$<$<CONFIG:Debug>:SPX_DEBUG>
	$<$<CONFIG:Release>:SPX_RELEASE>
	$<$<CONFIG:Shipping>:SPX_SHIPPING>
	$<$<CONFIG:Debug>:SPX_CONFIG_NAME="Debug">
	$<$<CONFIG:Release>:SPX_CONFIG_NAME="Release">
	$<$<CONFIG:Shipping>:SPX_CONFIG_NAME="Shipping">)
if (SPHYNX_BUILD_SHARED)
	target_compile_definitions(${PROJECT_NAME}  PUBLIC SPX_BUILD_SHARED)
else()
	target_compile_definitions(${PROJECT_NAME}  PUBLIC SPX_BUILD_STATIC)
endif()

if (WIN32)
	target_compile_definitions(${PROJECT_NAME}  PUBLIC SPX_PLATFORM_WINDOWS)
endif()

if (WIN32)
	target_compile_definitions(${PROJECT_NAME} PUBLIC 
		_CRT_SECURE_NO_WARNINGS
		_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS)
endif()

target_compile_definitions(${PROJECT_NAME}  PRIVATE SPHYNX_BUILD)