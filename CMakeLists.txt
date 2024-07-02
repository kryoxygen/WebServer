cmake_minimum_required(VERSION 3.28)
project(webserver VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)

aux_source_directory(./logger log)
message(STATUS ${log})

add_executable(webserver main.cpp ${log})

