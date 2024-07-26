cmake_minimum_required(VERSION 3.5.x)
project("logoswap")
add_executable(${PROJECT_NAME} main.c splash.c bitmap.c)
target_compile_options(${PROJECT_NAME} PUBLIC -std=c99 -O2 -Wall -pedantic -Wextra)