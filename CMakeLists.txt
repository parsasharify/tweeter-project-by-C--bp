cmake_minimum_required(VERSION 3.17)
project(test1comb C)

set(CMAKE_C_STANDARD 99)

add_executable(test1comb main.c cJSON.c)
target_link_libraries(test1comb ws2_32)