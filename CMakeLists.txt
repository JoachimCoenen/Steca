cmake_minimum_required (VERSION 3.1)
include (./cmake.options)

project (SteCa2)

if (DEVELOPMENT)
  add_definitions (-DDEVELOPMENT)
endif ()

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory (lib)
add_subdirectory (src)
add_subdirectory (tests)

# eof
