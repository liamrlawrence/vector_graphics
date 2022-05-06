cmake_minimum_required(VERSION 3.16)
project(vector_graphics C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-I/usr/include/cairo/ -L/usr/local/lib/ -lcairo")

add_executable(vector_graphics src/main.c src/drawing.c src/drawing.h src/subway.c src/subway.h)
target_link_libraries(vector_graphics cairo)
