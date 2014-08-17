# Tim Finer 
# tfiner@csu.fullerton.edu
# 
# This is a metafile used by CMake to generate a build file.  
# 

project (raytracer)

cmake_minimum_required (VERSION 2.6)

message( STATUS "Building Raytracer - ${CMAKE_BUILD_TYPE}")
message( STATUS "CMake detected OS '${CMAKE_SYSTEM_NAME}'" )

# My defaults
# set(CMAKE_C_COMPILER "/opt/local/bin/clang-mp-3.4")
# set(CMAKE_CXX_COMPILER "/opt/local/bin/clang++-mp-3.4")

message( STATUS "Building using '${CMAKE_CXX_COMPILER}'" )

# set (Boost_DEBUG ON)

# MacPorts is compiled for GCC, which is binary incompatible with
# Clang's libc++.  See:
# http://stackoverflow.com/a/8457799/196682
# find_package( Boost 1.54 COMPONENTS program_options REQUIRED )


find_package( ZLIB REQUIRED )
set( PNG_LIBRARY /Library/Frameworks/UnixImageIO.framework/unix/lib/libpng15.dylib )
set( PNG_INCLUDE_DIR  /Library/Frameworks/UnixImageIO.framework/unix/include )
message( STATUS "Found PNG: ")
message( STATUS "   version:  ${PNG_VERSION} ")
message( STATUS "   includes: ${PNG_INCLUDE_DIR} ")
message( STATUS "   libs:     ${PNG_LIBRARY} ")

# No cmake file for Eigen, have to use the path in ccmake.
# find_package(Eigen3 REQUIRED)

set(EIGEN3_INCLUDE_DIR "/opt/local/include/eigen3" CACHE FILEPATH "Path to Eigen3")
include_directories( "${EIGEN3_INCLUDE_DIR}" )

set(BOOST_INCLUDE_DIR "/opt/local/include/boost" CACHE FILEPATH "Path to Boost")
include_directories( ${Boost_INCLUDE_DIRS} )
message( STATUS "Boost inc: '${Boost_INCLUDE_DIRS}'" )

set(LIBNOISE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/libnoise/include")
include_directories( ${LIBNOISE_INCLUDE_DIR} )

set(NOISEUTILS_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/noiseutils")
include_directories( ${NOISEUTILS_INCLUDE_DIR} )


# Update the documentation string of CMAKE_BUILD_TYPE for GUIs
SET( CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING
"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
    FORCE )

set(CMAKE_CXX_FLAGS "-DUSE_TBB -std=c++11 -stdlib=libc++ -m64  -Wall -Woverloaded-virtual -Wextra -isystem /opt/local/include -isystem /usr/local/include" )
set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb3 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O0 -DNDEBUG")
# set_property(SOURCE src/scene_loader.cpp PROPERTY "-O0")

set(SRCS 
    src/ambient_occluder_light.cpp
    src/camera.cpp
    src/factory.cpp
    src/function.cpp
    src/geometry.cpp
    src/log_msg.cpp
    src/main.cpp
    src/matte.cpp
    src/memento.cpp
    src/noise_factory.cpp
    src/object.cpp
    src/orthogonal_camera.cpp
    src/point_light.cpp
    src/perspective_camera.cpp
    src/phong.cpp
    src/plane.cpp
    src/recorder_png.cpp
    src/sampler_uniform.cpp
    src/scene.cpp
    src/scene_loader.cpp
    src/scene_sampler.cpp
    src/simple_perspective_camera.cpp
    src/simplexnoise1234.cpp
    src/sphere.cpp
    src/starry_night.cpp
    src/unwind_dump.cpp
)

set(NOISE_UTIL
    noiseutils/noiseutils.cpp
    )

add_executable( raytracer ${SRCS} ${NOISE_UTIL} )
target_link_libraries( raytracer ${PNG_LIBRARY} tbb trng4 ${CMAKE_SOURCE_DIR}/libnoise/${CMAKE_BUILD_TYPE}/libnoise.a )

