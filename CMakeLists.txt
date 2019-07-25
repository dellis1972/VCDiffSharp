cmake_minimum_required (VERSION 2.8.12)

project (vc-diff CXX C)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/open-vcdiff/src)
link_directories(${CMAKE_SOURCE_DIR}/external/open-vcdiff/build/Darwin)

add_library(vc-diff SHARED src/vc-diff.cc)

target_link_libraries (vc-diff vcdcom vcddec vcdenc)