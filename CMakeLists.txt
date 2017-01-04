project (SteCa2)

cmake_minimum_required (VERSION 3.1 FATAL_ERROR)

set (CMAKE_CXX_STANDARD 11)
if (CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options("-Wall")
  if (WITH_COVERAGE)
    add_compile_options("--coverage")
  endif ()
endif ()

file (GLOB_RECURSE old_files ".old/*")
add_custom_target (.old SOURCES ${old_files})

add_subdirectory (c)
add_subdirectory (tests)

# eof
