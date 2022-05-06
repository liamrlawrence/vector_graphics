cmake_minimum_required(VERSION 3.22)
project(nether_subway C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "-I/usr/include/cairo/ -L/usr/local/lib/ -lcairo")

add_executable(nether_subway main.c drawing.c drawing.h subway.c subway.h)
target_link_libraries(nether_subway cairo)
