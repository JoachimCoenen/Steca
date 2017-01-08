project (SteCa2)

cmake_minimum_required (VERSION 3.1 FATAL_ERROR)

set (CMAKE_CXX_STANDARD 11)
if (CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options("-Wall -Wextra -Wpedantic")
  if (WITH_COVERAGE)
    add_compile_options("--coverage")
  endif ()
endif ()

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

file (GLOB_RECURSE old_files ".old/*")
add_custom_target (.old SOURCES ${old_files})

# Qt for now
add_definitions (-DWITH_QT)
set (CMAKE_AUTOMOC ON)
find_package (Qt5Core       REQUIRED)
find_package (Qt5Gui        REQUIRED)
find_package (Qt5Widgets    REQUIRED)

add_subdirectory (c)
add_subdirectory (tests)
add_subdirectory (test_app)

# eof
