cmake_minimum_required (VERSION 3.1)
include (../cmake.inc)

get_filename_component (app ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_definitions (-DWITH_TESTS)
if (WITH_COVERAGE)
  add_compile_options(--coverage)
endif ()

# include_directories (${CMAKE_HOME_DIRECTORY}/steca)

set (srcdirs dev_lib)
foreach (dir ${srcdirs})
  set (d ${CMAKE_HOME_DIRECTORY}/${dir})
  file (GLOB_RECURSE fs ${d}/*.cpp)
  set (files ${files} ${fs})
endforeach (dir)

add_executable (${app} ${files} ${files})

if (UNIX)
  target_link_libraries (${app} pthread)
endif ()

if (WITH_COVERAGE)
  target_link_libraries (${app} gcov)
endif ()

# eof
