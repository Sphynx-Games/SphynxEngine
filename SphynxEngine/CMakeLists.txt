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

# Get all my .cpp files and create variable CPPSOURCES
file(GLOB_RECURSE CPPSOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Set the excutable name and the main.cpp to be compiled
add_executable(SphynxEngine "${CMAKE_CURRENT_LIST_DIR}/src/main.cpp")

# Set target sources
target_sources(SphynxEngine PRIVATE ${CPPSOURCES})
target_include_directories(SphynxEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(SphynxEngine PUBLIC SPX_DEBUG) 
else()
	target_compile_definitions(SphynxEngine PUBLIC SPX_RELEASE) 
endif()

# Add a library folder
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/LIBRARY_FOLDER)

# Add include directories
# target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)

# Adds preprocessor definitions
# target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC GLFW_INCLUDE_NONE=1) 

# Links libraries
# target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glm)