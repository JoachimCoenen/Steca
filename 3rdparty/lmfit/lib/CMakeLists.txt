set(library_name lmfit)
set(${library_name}_LIBRARY ${library_name} PARENT_SCOPE)
set(${library_name}_LIBRARY_TYPE SHARED)

set(src_files lmcurve.c lmmin.c lminvert.c)
set(inc_files lmcurve.h lmmin.h lminvert.h lmstruct.h)

add_library(${library_name} SHARED ${src_files})

set_target_properties(
    ${library_name} PROPERTIES
    OUTPUT_NAME ${library_name}
    VERSION ${lmfit_VERSION}
    SOVERSION ${lmfit_SOVERSION})

target_link_libraries(${library_name} m)

#{Steca patch}: no installation
