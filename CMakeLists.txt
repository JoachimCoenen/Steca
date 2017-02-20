project (SteCa2)
cmake_minimum_required (VERSION 3.1 FATAL_ERROR)

set (CMAKE_CXX_STANDARD 11)
if (CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -Wextra -Wpedantic)

  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  endif()
endif ()

file (GLOB_RECURSE old_files .old/*)
add_custom_target (.old SOURCES ${old_files})

#file (GLOB_RECURSE ci_files .travis/*)
#add_custom_target (.ci SOURCES .travis.yml .appveyor.yml ${ci_files})

#file (GLOB_RECURSE sh_files *.sh)
#add_custom_target (.sh SOURCES ${sh_files})

set (txt_files AUTHORS CHANGELOG COPYING README.md VERSION manifest)
add_custom_target (.txt SOURCES ${txt_files})

include_directories (${CMAKE_HOME_DIRECTORY})

add_subdirectory (c)
add_subdirectory (STeCa)

if (WITH_TESTS)
  add_subdirectory (tests)
endif ()

# eof
