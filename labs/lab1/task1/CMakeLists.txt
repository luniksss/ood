cmake_minimum_required(VERSION 3.30)
project(lab1)

set(CMAKE_CXX_STANDARD 20)
enable_testing()

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/release-1.12.1.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(${PROJECT_NAME} SimUDuck.cpp)
add_executable(${PROJECT_NAME}_test test/SimUDuckTests.cpp)

target_link_libraries(${PROJECT_NAME}_test
        PRIVATE
        GTest::gtest_main
        GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)