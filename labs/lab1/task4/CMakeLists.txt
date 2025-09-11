cmake_minimum_required(VERSION 3.30)
project(task4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ./../lib)
set(SFML_ROOT "D:/Luniks/uni/ood/lab1/task4/SFML")
set(CMAKE_MODULE_PATH ${SFML_ROOT}/lib/cmake/SFML ${CMAKE_MODULE_PATH})
find_package(SFML 2.5 COMPONENTS graphics window system QUIET)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

option(CODE_COVERAGE "Enable code coverage" OFF)
if (CODE_COVERAGE)
    if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
    else ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
    endif ()
endif ()

set(SOURCE_FILES
        main.cpp
        ./service/DrawingController.cpp
        ./service/DrawingCommandHandler.cpp
        ./service/ShapeFactory.cpp
        ./picture/Picture.cpp
)

set(TEST_SOURCE_FILES
        test/ShapeTest.cpp
)

add_executable(task4 ${SOURCE_FILES})
target_link_libraries(task4
        PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
)

add_executable(task4_test ${TEST_SOURCE_FILES})
target_link_libraries(task4_test
        PRIVATE
        GTest::gtest_main
        sfml-graphics
)
enable_testing()
include(GoogleTest)
gtest_discover_tests(task4_test)