cmake_minimum_required(VERSION 3.30)
project(task2)

set(CMAKE_CXX_STANDARD 20)
enable_testing()

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/release-1.12.1.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(task2 SimUDuck.cpp)
add_executable(task2_test test/SimUDuckTest.cpp)

target_link_libraries(task2_test
        PRIVATE
        GTest::gtest_main
        GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(task2_test)