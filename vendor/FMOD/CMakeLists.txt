cmake_minimum_required(VERSION 3.6)

#if (CMAKE_BUILD_TYPE STREQUAL "Debug")
#	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
#else()
#	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
#endif()

#project(FMOD CXX)

set(FMOD_CORE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/core")
set(FMOD_STUDIO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/studio")
set(FMOD_FSBANK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/fsbank")

# Detect architecture
string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" FMOD_CPU)
if(FMOD_CPU MATCHES "arm64")
    set(FMOD_ARCH "arm64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(FMOD_ARCH "x64")
else()
    set(FMOD_ARCH "x86")
endif()

# Get the correct lib (Debug: sufix = L, otherwise: no sufix)
set(FMOD_CORE_BIN_DIR "${FMOD_CORE_DIR}/lib/${FMOD_ARCH}")
set(FMOD_STUDIO_BIN_DIR "${FMOD_STUDIO_DIR}/lib/${FMOD_ARCH}")

# Create imported libraries
add_library(FMOD::Core SHARED IMPORTED GLOBAL)
set_target_properties(FMOD::Core PROPERTIES
    IMPORTED_IMPLIB "${FMOD_CORE_BIN_DIR}/fmod_vc.lib"
    IMPORTED_LOCATION "${FMOD_CORE_BIN_DIR}/fmod.dll"
    IMPORTED_IMPLIB_DEBUG "${FMOD_CORE_BIN_DIR}/fmodL_vc.lib"
    IMPORTED_LOCATION_DEBUG "${FMOD_CORE_BIN_DIR}/fmodL.dll"
    INTERFACE_INCLUDE_DIRECTORIES "${FMOD_CORE_DIR}/inc"
)

add_library(FMOD::Studio SHARED IMPORTED GLOBAL)
set_target_properties(FMOD::Studio PROPERTIES
    IMPORTED_IMPLIB "${FMOD_STUDIO_BIN_DIR}/fmodstudio_vc.lib"
    IMPORTED_LOCATION "${FMOD_STUDIO_BIN_DIR}/fmodstudio.dll"
    IMPORTED_IMPLIB_DEBUG "${FMOD_STUDIO_BIN_DIR}/fmodstudioL_vc.lib"
    IMPORTED_LOCATION_DEBUG "${FMOD_STUDIO_BIN_DIR}/fmodstudioL.dll"
    INTERFACE_INCLUDE_DIRECTORIES "${FMOD_STUDIO_DIR}/inc"
)

# Group all the FMOD specific libraries into the FMOD interface
add_library(FMOD INTERFACE)
target_link_libraries(FMOD INTERFACE FMOD::Core FMOD::Studio)