set (lib c_qt)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/..)

file (GLOB_RECURSE files *)
add_library (${lib} ${files})

set (CMAKE_AUTOMOC ON)
find_package (Qt5Core    REQUIRED)
find_package (Qt5Gui     REQUIRED)
find_package (Qt5Widgets REQUIRED)

target_link_libraries (${lib}
  c_cpp c_c
  Qt5::Core Qt5::Gui Qt5::Widgets
)

#eof
