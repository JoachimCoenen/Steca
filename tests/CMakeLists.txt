get_filename_component (app ${CMAKE_CURRENT_SOURCE_DIR} NAME)
add_definitions (-DWITH_TESTS)

find_package (Qt5Core    REQUIRED)
find_package (Qt5Gui     REQUIRED)
find_package (Qt5Widgets REQUIRED)

set (CMAKE_AUTOMOC ON)

set (srcdirs STeCa c)
foreach (dir ${srcdirs})
  set (d ${CMAKE_HOME_DIRECTORY}/${dir})
  file (GLOB_RECURSE fs ${d}/*.cpp)
  set (files ${files} ${fs})
endforeach (dir)

list(REMOVE_ITEM files ${CMAKE_HOME_DIRECTORY}/STeCa/main.cpp)

include_directories (${CMAKE_HOME_DIRECTORY}/STeCa)

file (GLOB_RECURSE test_files *)

if (WITH_COVERAGE)
  add_compile_options(--coverage)
endif ()

add_executable (${app} ${files} ${test_files})

target_link_libraries (${app}
  Qt5::Core Qt5::Gui Qt5::Widgets
)

if (UNIX)
  target_link_libraries (${app} pthread)
endif ()

if (WITH_COVERAGE)
  target_link_libraries (${app} gcov)
endif ()

# eof
