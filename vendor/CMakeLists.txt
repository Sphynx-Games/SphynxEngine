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
set(GLM_BUILD_TESTS OFF)

# Add directories of the different libraries
add_subdirectory(SDL SDL/build)
add_subdirectory(glm glm/build)

set_all_targets_in_folder(SDL vendor/SDL)
set_all_targets_in_folder(glm vendor/glm)

# Link to the diferent libraries
target_link_libraries(vendor INTERFACE
	SDL3::SDL3)
target_link_libraries(vendor INTERFACE
	glm::glm)
