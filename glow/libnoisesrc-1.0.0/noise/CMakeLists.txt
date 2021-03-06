cmake_minimum_required(VERSION 2.6)

set(SRCS 
    src/noisegen.cpp
    src/latlon.cpp
    src/model/cylinder.cpp
    src/model/line.cpp
    src/model/plane.cpp
    src/model/sphere.cpp
    src/module/abs.cpp
    src/module/add.cpp
    src/module/billow.cpp
    src/module/blend.cpp
    src/module/cache.cpp
    src/module/checkerboard.cpp
    src/module/clamp.cpp
    src/module/const.cpp
    src/module/curve.cpp
    src/module/cylinders.cpp
    src/module/displace.cpp
    src/module/exponent.cpp
    src/module/invert.cpp
    src/module/max.cpp
    src/module/min.cpp
    src/module/modulebase.cpp
    src/module/multiply.cpp
    src/module/perlin.cpp
    src/module/power.cpp
    src/module/ridgedmulti.cpp
    src/module/rotatepoint.cpp
    src/module/scalebias.cpp
    src/module/scalepoint.cpp
    src/module/select.cpp
    src/module/spheres.cpp
    src/module/terrace.cpp
    src/module/translatepoint.cpp
    src/module/turbulence.cpp
    src/module/voronoi.cpp
)


add_library(noise STATIC ${SRCS})
SET_TARGET_PROPERTIES(noise PROPERTIES COMPILE_FLAGS "-fPIC")