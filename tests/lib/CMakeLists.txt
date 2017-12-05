cmake_minimum_required (VERSION 3.1)
include (../../cmake.options)

set (app tests_lib)

add_definitions (-DWITH_TESTS)

set (srcdirs lib/dev)
foreach (dir ${srcdirs})
  set (d ${CMAKE_HOME_DIRECTORY}/${dir})
  file (GLOB_RECURSE fs ${d}/*.cpp)
  set (files ${files} ${fs})
endforeach (dir)

add_executable (${app} ${files} ${files})

if (UNIX)
  target_link_libraries (${app} pthread)
endif ()

# eof
