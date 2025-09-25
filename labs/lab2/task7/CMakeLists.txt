cmake_minimum_required(VERSION 3.30)
project(task7)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ./../lib)

#include(FetchContent)
#FetchContent_Declare(
#        googletest
#        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
#)
#FetchContent_MakeAvailable(googletest)
#
#option(CODE_COVERAGE "Enable code coverage" OFF)
#if (CODE_COVERAGE)
#    if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
#        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
#    else ()
#        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
#    endif ()
#endif ()

set(SOURCE_FILES
        ./main.cpp
)

add_executable(task7 ${SOURCE_FILES})

#add_executable(task5_test ${TEST_SOURCE_FILES})
#target_link_libraries(task5_test
#        PRIVATE
#        GTest::gtest_main
#)
#enable_testing()
#include(GoogleTest)
#gtest_discover_tests(task5_test)