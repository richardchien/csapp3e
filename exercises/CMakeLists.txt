cmake_minimum_required(VERSION 3.14)
project(csapp)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.c)
add_executable(csapp ${SOURCE_FILES})
target_link_libraries(csapp pthread)
