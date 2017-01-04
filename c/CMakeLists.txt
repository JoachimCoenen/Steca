get_filename_component(lib ${CMAKE_CURRENT_SOURCE_DIR} NAME)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

file (GLOB_RECURSE files "*")
add_library (${lib} ${files})

#eof
