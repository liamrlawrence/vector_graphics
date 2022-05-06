cmake_minimum_required(VERSION 3.16)
project(nether_subway C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "-I/usr/include/cairo/ -L/usr/local/lib/ -lcairo")

add_executable(nether_subway src/main.c src/drawing.c src/drawing.h src/subway.c src/subway.h)
target_link_libraries(nether_subway cairo)
