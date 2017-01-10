get_filename_component (app ${CMAKE_CURRENT_SOURCE_DIR} NAME)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/..)

add_definitions (-DWITH_TESTS)

set (srcdirs c)

foreach (dir ${srcdirs})
  set (d ${CMAKE_HOME_DIRECTORY}/${dir})
  file (GLOB_RECURSE fs ${d}/*.cpp)
  set (files ${files} ${fs})
endforeach (dir)

file (GLOB_RECURSE test_files *)

add_executable (${app}
  ${files} ${test_files}
)

# eof
