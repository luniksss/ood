cmake_minimum_required(VERSION 3.30)
project(task1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ./../lib)

set(SOURCE_FILES
        main.cpp
)

add_executable(task1 ${SOURCE_FILES})