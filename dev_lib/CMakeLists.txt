cmake_minimum_required (VERSION 3.1)
include (../cmake.inc)

get_filename_component(lib ${CMAKE_CURRENT_SOURCE_DIR} NAME)

file (GLOB_RECURSE lib_src inc/* typ/* io/*)
add_library (${lib} ${lib_src})

# eof
