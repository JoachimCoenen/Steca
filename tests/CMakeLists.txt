get_filename_component (app ${CMAKE_CURRENT_SOURCE_DIR} NAME)
add_definitions (-DWITH_TESTS)

TODO

find_package (Qt5Core    REQUIRED)
find_package (Qt5Gui     REQUIRED)
find_package (Qt5Widgets REQUIRED)

file (GLOB_RECURSE test_files *)
add_executable (${app} ${test_files})

target_link_libraries (${app}
  gui core c_qt c_cpp c_c
  Qt5::Core Qt5::Gui Qt5::Widgets
)

# eof
