cmake_minimum_required(VERSION 3.10)
project(mySimpleMessageClient C)

set(CMAKE_C_STANDARD 99)

add_executable(mySimpleMessageClient main.c logger.c logger.h)