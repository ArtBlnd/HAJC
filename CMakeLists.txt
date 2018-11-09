cmake_minimum_required(VERSION 3.5)
project(HAJC)


# Enabling C++ 17 by adding compiler flags
# MSVC uses /std:c++17, and another all such as GCC, Clang uses -std=c++17
if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
else()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

add_subdirectory(src)
add_subdirectory(tests)
