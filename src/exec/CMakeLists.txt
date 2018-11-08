add_executable(hlangc hlangc.cpp)
add_dependencies(hlangc tools compiler language)
target_link_libraries(hlangc tools compiler language)