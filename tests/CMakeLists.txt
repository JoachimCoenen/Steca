get_filename_component (app ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_definitions (-DWITH_TESTS)

set (srcdirs c)

foreach (dir ${srcdirs})
  set (sd ${CMAKE_HOME_DIRECTORY}/${dir})
  include_directories (${sd})
  file (GLOB_RECURSE fs ${sd}/*.cpp)
  set (files ${files} ${fs})
endforeach (dir)

file (GLOB_RECURSE test_files *.h *.cpp)

add_executable (${app}
  ${files} ${test_files}
)

# eof
