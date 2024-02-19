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
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COPILER "msvc")

set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin-int/${PROJECT_NAME}/$<CONFIG>)
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin-int/${PROJECT_NAME}/$<CONFIG>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}/$<CONFIG>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}/$<CONFIG>)

# Get all my .cpp files and create variable SPXSOURCES 
file(GLOB_RECURSE SPXSOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp" 
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" )

# Set the library name and the sources to be compiled
add_library(${PROJECT_NAME} SHARED ${SPXSOURCES})

# Set target sources
target_sources(${PROJECT_NAME} PRIVATE ${SPXSOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/")
target_link_libraries(${PROJECT_NAME} PRIVATE vendor)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(${PROJECT_NAME} PUBLIC SPX_DEBUG) 
else()
	target_compile_definitions(${PROJECT_NAME} PUBLIC SPX_RELEASE) 
endif()

if (WIN32)
	target_compile_definitions(${PROJECT_NAME} PUBLIC SPX_BUILD_DLL)
endif()

# Add a library folder
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/LIBRARY_FOLDER)

# Add include directories
# target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)

# Adds preprocessor definitions
# target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC GLFW_INCLUDE_NONE=1) 

# Links libraries
# target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glm)