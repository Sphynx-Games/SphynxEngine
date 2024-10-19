cmake_minimum_required(VERSION 3.6)

function(set_all_targets_in_folder TARGETDIR FOLDERDIR)
	get_property(TGTS DIRECTORY "${TARGETDIR}" PROPERTY BUILDSYSTEM_TARGETS)
	foreach(TGT IN LISTS TGTS)
		message(STATUS "Target: ${TGT}")
		set_property(TARGET ${TGT} PROPERTY FOLDER "${FOLDERDIR}")
	endforeach()

	get_property(SUBDIRS DIRECTORY "${TARGETDIR}" PROPERTY SUBDIRECTORIES)
	foreach(SUBDIR IN LISTS SUBDIRS)
		set_all_targets_in_folder("${SUBDIR}" "${FOLDERDIR}/${SUBDIR}")
	endforeach()
endfunction()

# Create imgui project from this file
project(imgui CXX)

# Get all my .cpp files and create variable IMGUISOURCES 
file(GLOB IMGUISOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.hpp" 
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.cpp" 
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui/misc/cpp/imgui_stdlib.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui/misc/cpp/imgui_stdlib.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_sdl3.h" 
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_sdlrenderer3.h" 
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_sdl3.cpp" 
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_sdlrenderer3.cpp"
)

add_library(imgui STATIC ${IMGUISOURCES})

target_sources(imgui PRIVATE ${IMGUISOURCES})
target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/imgui/")
target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/")
target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/SDL/include/")

# Create interface (all libraries will be linked to "vendor" interface), so the main project can only link to vendor
add_library(vendor INTERFACE)


# Add directories of the different libraries
# Add UUID Library
set(UUID_BUILD_TESTS OFF)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
add_subdirectory(stduuid)

# Add SDL3 Library, SDL3_Image, SDL3_ttf
set(SDL_TESTS OFF)
set(SDLIMAGE_VENDORED OFF)
add_subdirectory(SDL)
add_subdirectory(SDL_image)
add_subdirectory(SDL_ttf)

# Force static library builds
set(BUILD_SHARED_LIBS OFF)

# Add GLM Library
set(GLM_ENABLE_CXX_17 ON)
add_subdirectory(glm)

# Add SPDLOG Library
add_subdirectory(spdlog)

# Add EnTT Library
add_subdirectory(entt)

# Add Box2D Library
set(BOX2D_BUILD_UNIT_TESTS OFF)
set(BOX2D_BUILD_TESTBED OFF)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
add_subdirectory(box2d)

# Configure filters (folders)
set_property(TARGET stduuid PROPERTY FOLDER vendor/stduuid)
set_all_targets_in_folder(SDL vendor/SDL)
set_all_targets_in_folder(SDL_image vendor/SDL)
set_all_targets_in_folder(SDL_ttf vendor/SDL)
set_property(TARGET glm PROPERTY FOLDER vendor/glm)
set_all_targets_in_folder(spdlog vendor/spdlog)
set_all_targets_in_folder(entt vendor/entt)
set_property(TARGET box2d PROPERTY FOLDER vendor/box2d)
set_property(TARGET imgui PROPERTY FOLDER vendor/imgui)

# Link to the diferent libraries
target_link_libraries(vendor INTERFACE
	stduuid
	SDL3::SDL3
	SDL3_image::SDL3_image
	SDL3_ttf::SDL3_ttf
	glm::glm
	spdlog::spdlog
	EnTT::EnTT
	box2d
	imgui)
