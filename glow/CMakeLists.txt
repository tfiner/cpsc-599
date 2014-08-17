# Tim Finer 
# tfiner@csu.fullerton.edu
# 
# This is a metafile used by CMake to generate a build file.  
# 

project (glow)

cmake_minimum_required (VERSION 2.6)

message( STATUS "Building Glow - ${CMAKE_BUILD_TYPE}")
message( STATUS "CMake detected OS '${CMAKE_SYSTEM_NAME}'" )


message( STATUS "Building using '${CMAKE_CXX_COMPILER}'" )

# Update the documentation string of CMAKE_BUILD_TYPE for GUIs
SET( CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING
"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
    FORCE )


# set (Boost_DEBUG ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/modules)

find_package( ZLIB REQUIRED )
set( PNG_LIBRARY /Library/Frameworks/UnixImageIO.framework/unix/lib/libpng15.dylib )
set( PNG_INCLUDE_DIR  /Library/Frameworks/UnixImageIO.framework/unix/include )
message( STATUS "Found PNG: ")
message( STATUS "   version:  ${PNG_VERSION} ")
message( STATUS "   includes: ${PNG_INCLUDE_DIR} ")
message( STATUS "   libs:     ${PNG_LIBRARY} ")

find_package(PythonDev REQUIRED)
include_directories( ${PYTHON_INCLUDE_DIR} )

set(EIGEN3_INCLUDE_DIR "/usr/local/include/eigen3" CACHE FILEPATH "Path to Eigen3")
include_directories( "${EIGEN3_INCLUDE_DIR}" )

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.55.0 COMPONENTS python) 
message( STATUS "Boost libs: '${Boost_LIBRARIES}'" )

set(BOOST_INCLUDE_DIR "/usr/local/include/boost" CACHE FILEPATH "Path to Boost")
include_directories( ${Boost_INCLUDE_DIRS} )
message( STATUS "Boost inc: '${Boost_INCLUDE_DIRS}'" )

set(LIBNOISE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/libnoisesrc-1.0.0/noise/include")
set(LIBNOISE_LIB_DIR "${CMAKE_SOURCE_DIR}/libnoisesrc-1.0.0/noise/${CMAKE_BUILD_TYPE}")
include_directories( ${LIBNOISE_INCLUDE_DIR} )

set(NOISEUTILS_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/noiseutils")
include_directories( ${NOISEUTILS_INCLUDE_DIR} )

set(CMAKE_CXX_FLAGS "-DUSE_TBB -std=c++11 -stdlib=libc++ -m64  -Wall -Woverloaded-virtual -Wextra -isystem /usr/local/include -fdiagnostics-fixit-info" )
set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb3 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O0 -DNDEBUG")

set(SRCS 
    src/ambient_occluder_light.cpp
    src/camera.cpp
    src/factory.cpp
    src/function.cpp
    src/geometry.cpp
    src/loader.cpp
    src/log_msg.cpp
    src/matte.cpp
    src/memento.cpp
    src/noise_factory.cpp
    src/object.cpp
    src/orthogonal_camera.cpp
    src/point_light.cpp
    src/perspective_camera.cpp
    src/phong.cpp
    src/plane.cpp
    src/pyglow.cpp
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

set(GLO_SRC
    src/main.cpp
)


# libglow.dynlib or .so or .dll
add_library( glow SHARED ${SRCS} ${NOISE_UTIL} )
target_link_libraries( glow ${PNG_LIBRARY} tbb trng4 ${LIBNOISE_LIB_DIR}/libnoise.a ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})

# glo, the executable
add_executable( glo ${GLO_SRC} )
target_link_libraries( glo glow )

