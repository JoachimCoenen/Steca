add_definitions (-DWITH_TESTS)

set (srcdirs
  c
)

foreach (dir ${srcdirs})
  include_directories (${CMAKE_HOME_DIRECTORY}/${dir})
  file (GLOB_RECURSE fs ${CMAKE_HOME_DIRECTORY}/${dir}/*.cpp)
  set (files ${files} ${fs})
endforeach (dir)

file (GLOB_RECURSE test_files *.h *.cpp)

add_executable (tests
  ${test_files}
  ${files}
)

# eof
