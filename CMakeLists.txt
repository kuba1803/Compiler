cmake_minimum_required(VERSION 3.22)
project(Compiler)

set(CMAKE_CXX_STANDARD 23)

add_executable(Compiler main.cpp Utils/Source.h)
add_subdirectory(Lexer)
