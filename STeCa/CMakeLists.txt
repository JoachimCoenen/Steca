get_filename_component (app ${CMAKE_CURRENT_SOURCE_DIR} NAME)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/..)

file (GLOB_RECURSE files *)
add_executable (${app} ${files})

find_package (Qt5Core    REQUIRED)
find_package (Qt5Gui     REQUIRED)
find_package (Qt5Widgets REQUIRED)

target_link_libraries (${app}
  gui core c_qt c_cpp c_c
  Qt5::Core Qt5::Gui Qt5::Widgets
)

# eof
