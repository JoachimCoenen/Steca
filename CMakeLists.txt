# Steca top-level configuration

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake
    ${CMAKE_SOURCE_DIR}/cmake/steca/modules)
include(PreventInSourceBuilds)

project(Steca)

option(WITH_TESTS "Build with all tests" ON)

macro(SET_OPTION option value)
    set(${option} ${value} CACHE INTERNAL "" FORCE)
endmacro()
SET_OPTION(LIB_MAN OFF)
SET_OPTION(LIB_INSTALL OFF)
SET_OPTION(CERF_CPP ON)

if(CMAKE_VERSION VERSION_LESS "3.1")
    set(CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_CXX_FLAGS}")
    set(CMAKE_C_FLAGS   "-std=c99   ${CMAKE_CXX_FLAGS}")
else()
    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_C_STANDARD   99)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__STRICT_ANSI__ -DQT_LOGGING_RULES=\"*.debug=true\"")
if (CERF_CPP)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCERF_AS_CPP")
endif()

if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS}")
    set(MS_NOWARN "/wd4267 /wd4305 /wd4244 /wd4068 /wd4996 /wd4101 /wd4715")
    set(MS_D "-D_CRT_SECURE_NO_WARNINGS -D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${MS_NOWARN} ${MS_D}")
else()
    # -fsanitize=address -fsanitize=undefined
    set(DEB_FLAGS "-Wall -Wno-sign-compare -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${DEB_FLAGS} -O0 -g")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3 -w")
endif()

find_package(Qt5Core    REQUIRED)
find_package(Qt5Gui     REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(YAML       REQUIRED)
message(STATUS "found Qt5Core=${Qt5Core_VERSION_STRING}, Qt5Gui=${Qt5Gui_VERSION_STRING}")
message(STATUS "Qt5Core=${Qt5Core_DIR}, Qt5=${Qt5Gui_DIR}")

add_subdirectory(3rdparty/LevMar)
add_subdirectory(3rdparty/QCustomPlot)
add_subdirectory(3rdparty/Caress)
add_subdirectory(3rdparty/Mar)
add_subdirectory(3rdparty/libcerf)
# add_subdirectory(3rdparty/lmfit)
add_subdirectory(qcr)
add_subdirectory(core)
add_subdirectory(gui)
add_subdirectory(main)

if(WITH_TESTS)
    include(CTest) # equivalent to "enable_testing() ???
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}) # => 'make check' is alias for 'ctest'
    set(gtest_force_shared_crt ON
        CACHE bool "Use shared (DLL) run-time lib even when Google Test is built as static lib.")
    add_subdirectory(3rdparty/gtest)
    configure_file(testdata/testdata.h.in ${CMAKE_BINARY_DIR}/testdata.h @ONLY)
    add_subdirectory(utest)
else()
    message(STATUS "Skipping tests")
endif()
