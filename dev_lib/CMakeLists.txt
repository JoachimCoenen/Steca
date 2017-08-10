cmake_minimum_required (VERSION 3.1)
include (../cmake.options)

get_filename_component(lib ${CMAKE_CURRENT_SOURCE_DIR} NAME)

file (GLOB lib_src defs.* str.* inc/* typ/* io/*)
add_library (${lib} ${lib_src})

# eof
