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

set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin-int/$<CONFIG>/)
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin-int/$<CONFIG>/)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Binaries/$<CONFIG>/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Binaries/$<CONFIG>/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Binaries/$<CONFIG>/)

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

target_compile_definitions(Sandbox PRIVATE SANDBOX_BUILD_MODULE)

# Set Sandbox target properties
set(COMMAND_ARGUMENTS "-d ${CMAKE_CURRENT_SOURCE_DIR} -m Sandbox")

set_target_properties(Sandbox PROPERTIES
    VS_DEBUGGER_COMMAND "$<$<CONFIG:Debug>:$<TARGET_FILE:SphynxEngine-Editor>>\
$<$<CONFIG:Release>:$<TARGET_FILE:SphynxEngine-Editor>>\
$<$<CONFIG:Shipping>:$<TARGET_FILE:SphynxEngine-Runtime>>"
    VS_DEBUGGER_COMMAND_ARGUMENTS "$<$<CONFIG:Debug>:>\
$<$<CONFIG:Release>:>\
$<$<CONFIG:Shipping>:${COMMAND_ARGUMENTS}>"
    #VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)