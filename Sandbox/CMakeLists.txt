# ---------------------------------------------------
# Sandbox Project CMake Setup
# ---------------------------------------------------
cmake_minimum_required(VERSION 3.6)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
else()
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif()

project(Sandbox CXX)

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

# Set the dll (shared or dynamic library) name and the files to be compiled
add_library(Sandbox SHARED "${SNBSOURCES}")

# Set target sources
target_sources(Sandbox PRIVATE ${SNBSOURCES})

target_link_libraries(Sandbox PRIVATE SphynxEngine)

# Post build command (copy .dll to bin folder)
add_custom_target(SandboxCopyBinaries 
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SphynxEngine> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}

	COMMENT "Copying dependency binaries to '${CMAKE_RUNTIME_OUTPUT_DIRECTORY}'" VERBATIM
)
add_dependencies(Sandbox SandboxCopyBinaries)
set_target_properties(SandboxCopyBinaries PROPERTIES FOLDER __internal__)

target_compile_definitions(Sandbox PRIVATE SANDBOX_BUILD_MODULE)