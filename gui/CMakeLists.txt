get_filename_component (app ${CMAKE_CURRENT_SOURCE_DIR} NAME)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/..)

file (GLOB_RECURSE files *)
add_executable (${app} ${files})

set (CMAKE_AUTOMOC ON)
find_package (Qt5Core    REQUIRED)
find_package (Qt5Gui     REQUIRED)
find_package (Qt5Widgets REQUIRED)

target_link_libraries (${app}
  core c2c
  Qt5::Core Qt5::Gui Qt5::Widgets
)

# eof
