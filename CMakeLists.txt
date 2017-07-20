cmake_minimum_required (VERSION 3.1)
include (./cmake.inc)

project (SteCa2)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory (dev_lib)
add_subdirectory (qt_lib)
add_subdirectory (steca)

add_subdirectory (tests_lib)
add_subdirectory (tests_steca)

#file (GLOB_RECURSE other_files .*.sh)
#file (GLOB_RECURSE other_files * .*)
file (GLOB_RECURSE other_files to-add/*)
add_custom_target (other SOURCES ${other_files})

# eof
