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


# Create interface (all libraries will be linked to "vendor" interface), so the main project can only link to vendor
add_library(vendor INTERFACE)

# Set definitions
set(SDL_TESTS OFF)
set(SDL3IMAGE_VENDORED OFF)
set(GLM_ENABLE_CXX_17 ON)

# Add directories of the different libraries
add_subdirectory(SDL SDL/build)
add_subdirectory(SDL_image SDL_image/build)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(glm glm/build)
add_subdirectory(spdlog spdlog/build)
add_subdirectory(entt entt/build)

set_all_targets_in_folder(SDL vendor/SDL)
set_all_targets_in_folder(SDL_image vendor/SDL)
# set in folder (function is not working propertly)
set_property(TARGET glm PROPERTY FOLDER vendor/glm)
set_all_targets_in_folder(spdlog vendor/spdlog)
set_all_targets_in_folder(entt vendor/entt)

# Link to the diferent libraries
target_link_libraries(vendor INTERFACE
	SDL3::SDL3
	SDL3_image::SDL3_image
	glm::glm
	spdlog::spdlog
	EnTT::EnTT)
