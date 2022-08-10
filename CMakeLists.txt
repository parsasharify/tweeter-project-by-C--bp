cmake_minimum_required(VERSION 3.17)
project(untitled7 C)

set(CMAKE_C_STANDARD 11)

add_executable(untitled7 main.c cJSON.c)
target_link_libraries(untitled7 ws2_32)