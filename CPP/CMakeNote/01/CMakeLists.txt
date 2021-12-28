cmake_minimum_required(VERSION 3.12)
project(hellocmake LANGUAGES CXX)

add_library(hellolib STATIC HelloWord.cpp )
add_executable(hello main.cpp)
target_link_libraries(hello PUBLIC hellolib)
