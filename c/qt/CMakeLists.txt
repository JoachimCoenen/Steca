set (lib c_qt)

find_package (Qt5Core    REQUIRED)
find_package (Qt5Gui     REQUIRED)
find_package (Qt5Widgets REQUIRED)

set (CMAKE_AUTOMOC ON)

file (GLOB_RECURSE files *)
add_library (${lib} ${files})

target_link_libraries (${lib}
  Qt5::Core Qt5::Gui Qt5::Widgets
)

#eof
