############################################################################
# top-level CMakeLists.txt for building STeCa
############################################################################

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

project(STeCa)

# --- The version number ---
set(STeCa_VERSION_MAJOR 2)
set(STeCa_VERSION_MINOR 0)
set(STeCa_VERSION_PATCH 0)

# --- Global settings ---
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# --- Check for required packages ---
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5Test REQUIRED)
find_package(Qt5Widgets REQUIRED)

# --- Recurse into the given subdirectories ---
add_subdirectory(core)
add_subdirectory(gui)
add_subdirectory(tests)