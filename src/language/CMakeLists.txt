add_library(language 
    Parser.cpp
    Parser.h)

add_dependencies(language compiler tools)
target_link_libraries(language compiler tools)