# ---------------------------------------------------
# SphynxEngine-Runtime Project CMake Setup
# ---------------------------------------------------
cmake_minimum_required(VERSION 3.6)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
else()
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif()

project(SphynxEngine-Runtime CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COPILER "msvc")

set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin-int/${PROJECT_NAME}/$<CONFIG>/)
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin-int/${PROJECT_NAME}/$<CONFIG>/)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}/$<CONFIG>/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}/$<CONFIG>/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}/$<CONFIG>/)

# Get all my .cpp files and create variable SNBSOURCES 
file(GLOB_RECURSE SNBSOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp" 
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" )

# Set the excutable name and the main.cpp to be compiled
add_executable(SphynxEngine-Runtime "${SNBSOURCES}")

# Set target sources
target_sources(SphynxEngine-Runtime PRIVATE ${CPPSOURCES})
target_include_directories(SphynxEngine-Runtime PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/")

# Set target link libs
target_link_libraries(SphynxEngine-Runtime SphynxEngine)
# TODO: remove this libraries below when possible
target_link_libraries(SphynxEngine-Runtime spdlog::spdlog)
target_link_libraries(SphynxEngine-Runtime glm::glm)
target_link_libraries(SphynxEngine-Runtime EnTT::EnTT)

# Post build command (copy .dll to bin folder)
add_custom_target(SphynxEngine-RuntimeCopyBinaries 
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SphynxEngine> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SDL3::SDL3> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SDL3_image::SDL3_image> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SDL3_ttf::SDL3_ttf> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}

	COMMENT "Copying dependency binaries to '${CMAKE_RUNTIME_OUTPUT_DIRECTORY}'" VERBATIM
)
add_dependencies(SphynxEngine-Runtime SphynxEngine-RuntimeCopyBinaries)
set_target_properties(SphynxEngine-RuntimeCopyBinaries PROPERTIES FOLDER __internal__)

target_compile_definitions(SphynxEngine-Runtime PUBLIC
	$<$<CONFIG:Debug>:SPX_DEBUG>
	$<$<CONFIG:RelWithDebInfo>:SPX_DEBUG>
	$<$<CONFIG:Release>:SPX_RELEASE>
	$<$<CONFIG:MinSizeRel>:SPX_RELEASE>) 

# Add a library folder
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/LIBRARY_FOLDER/)

# Add include directories
# target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)

# Adds preprocessor definitions
# target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC GLFW_INCLUDE_NONE=1) 

# Links libraries
# target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glm)