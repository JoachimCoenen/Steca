cmake_minimum_required (VERSION 3.1)
include (../../cmake.options)

set (lib lib_dev)

file (GLOB lib_src defs.* str.* inc/* typ/* io/* test/*)
add_library (${lib} ${lib_src})

# eof
