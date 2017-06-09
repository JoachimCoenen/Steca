cmake_minimum_required (VERSION 3.1)
include (../cmake.inc)

get_filename_component(lib ${CMAKE_CURRENT_SOURCE_DIR} NAME)

file (GLOB_RECURSE lib_src inc/* typ/*)
add_library (${lib} ${lib_src})

if (WITH_TESTS)
  add_definitions (-DWITH_TESTS)
  if (WITH_COVERAGE)
    add_compile_options(--coverage)
  endif ()

  set (test_app dev_lib_tests)

  file (GLOB_RECURSE test_src *)
  add_executable (${test_app} ${test_src})

  if (UNIX)
    target_link_libraries (${test_app} pthread)
  endif ()

  if (WITH_COVERAGE)
    target_link_libraries (${test_app} gcov)
  endif ()
endif ()

# eof
