cmake_minimum_required(VERSION 3.16)
project(project VERSION 1.0 LANGUAGES CXX C)

add_executable(MAIN.x src/main.c)

add_library(Timer inc/Timer.hpp)

target_link_libraries(MAIN.x Timaer)
