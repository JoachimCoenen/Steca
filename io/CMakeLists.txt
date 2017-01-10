get_filename_component (lib ${CMAKE_CURRENT_SOURCE_DIR} NAME)
file (GLOB_RECURSE files "*")
add_library (${lib} ${files})

# eof
