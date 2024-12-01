cmake_minimum_required(VERSION 3.6)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
else()
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif()

project(glad CXX)

# Get all my .cpp files and create variable GLADSOURCES 
file(GLOB_RECURSE GLADSOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp" 
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(glad STATIC ${GLADSOURCES})
set_target_properties(glad PROPERTIES LINKER_LANGUAGE CXX)

target_sources(glad PRIVATE ${GLADSOURCES})
target_include_directories(glad PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")