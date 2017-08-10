cmake_minimum_required (VERSION 3.1)
include (../../cmake.options)

if (CMAKE_COMPILER_IS_GNUCXX)
  # suppress warnings for legacy code
  add_compile_options(-Wno-all)
endif ()

get_filename_component (lib ${CMAKE_CURRENT_SOURCE_DIR} NAME)

file (GLOB_RECURSE files *)
add_library (${lib} ${files})

target_link_libraries (${lib}
)

# eof

