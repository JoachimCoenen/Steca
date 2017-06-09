cmake_minimum_required (VERSION 3.1)
include (./cmake.inc)

project (SteCa2)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory (dev_lib)
#add_subdirectory (steca)

file (GLOB_RECURSE md_files *.md)
add_custom_target (.all SOURCES ${md_files})

# eof
