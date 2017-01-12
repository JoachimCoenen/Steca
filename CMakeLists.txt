project (SteCa2)

cmake_minimum_required (VERSION 3.1 FATAL_ERROR)

set (CMAKE_CXX_STANDARD 11)
if (CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options("-Wall -Wextra -Wpedantic")
  if (WITH_COVERAGE)
    add_compile_options("--coverage")
  endif ()
endif ()

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

file (GLOB_RECURSE old_files .old/*)
add_custom_target (.old SOURCES ${old_files})

set (txt_files AUTHORS CHANGELOG COPYING README.md VERSION manifest)
add_custom_target (.txt SOURCES ${txt_files})

# light libraries
add_subdirectory (c)        # foundation
add_subdirectory (cpp)      # foundation < c
add_subdirectory (qt)       # qt-specific < cpp, c

# steca
add_subdirectory (core)     # < c cpp
add_subdirectory (gui)      # < qt

add_subdirectory (tests)
add_subdirectory (STeCa)

# eof
