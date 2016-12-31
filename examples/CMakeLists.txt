project (examples)
cmake_minimum_required (VERSION 2.8)

add_library(foo SHARED bar.h foo.h foo.cpp)
add_library(bar SHARED bar.h bar.cpp)
