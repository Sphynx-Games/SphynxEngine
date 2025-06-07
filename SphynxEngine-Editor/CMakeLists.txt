# ---------------------------------------------------
# SphynxEngine-Editor Project CMake Setup
# ---------------------------------------------------
cmake_minimum_required(VERSION 3.6)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
else()
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif()

project(SphynxEngine-Editor CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COPILER "msvc")

set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin-int/${PROJECT_NAME}/$<CONFIG>/)
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin-int/${PROJECT_NAME}/$<CONFIG>/)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}/$<CONFIG>/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}/$<CONFIG>/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}/$<CONFIG>/)

# Get all my .cpp files and create variable SPXESOURCES 
file(GLOB_RECURSE SPXESOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp" 
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" )

# Set the excutable name and the main.cpp to be compiled
add_executable(SphynxEngine-Editor "${SPXESOURCES}")

# Set target sources
target_sources(SphynxEngine-Editor PRIVATE ${CPPSOURCES})
target_include_directories(SphynxEngine-Editor PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/")

# Set target link libs
target_link_libraries(SphynxEngine-Editor SphynxEngine)

target_compile_definitions(SphynxEngine-Editor PRIVATE SPHYNX_EDITOR_BUILD)

# Post build command (copy .dll to bin folder)
add_custom_target(SphynxEngine-EditorRules 
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SphynxEngine> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SDL3::SDL3> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SDL3_image::SDL3_image> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SDL3_ttf::SDL3_ttf> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}

	COMMENT "Copying dependency binaries to '${CMAKE_RUNTIME_OUTPUT_DIRECTORY}'" VERBATIM
)
add_dependencies(SphynxEngine-Editor SphynxEngine-EditorRules)
set_target_properties(SphynxEngine-EditorRules PROPERTIES FOLDER __internal__)