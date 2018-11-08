include_directories(./)

add_definitions(-D__HAJC_WINDOWS__)

add_subdirectory(compiler)
add_subdirectory(datatypes)
add_subdirectory(exec)
add_subdirectory(language)
add_subdirectory(tools)