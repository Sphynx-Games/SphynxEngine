cmake_minimum_required(VERSION 3.6)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
else()
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif()

project(stb_image CXX)

# Get all my .cpp files and create variable STBIMGSOURCES 
file(GLOB_RECURSE STBIMGSOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.hpp" 
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(stb_image STATIC ${STBIMGSOURCES})
set_target_properties(stb_image PROPERTIES LINKER_LANGUAGE CXX)

target_sources(stb_image PRIVATE ${STBIMGSOURCES})
target_include_directories(stb_image PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/")