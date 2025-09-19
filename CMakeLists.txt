cmake_minimum_required(VERSION 3.20)
project(2q_cache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(cache_2q src/main_2q.cpp)
target_include_directories(cache_2q PRIVATE include)

add_executable(cache_belady src/main_belady.cpp)
target_include_directories(cache_belady PRIVATE include)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.tar.gz
)

set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(cache_2q_tests tests/cache_2q_tests.cpp)
target_include_directories(cache_2q_tests PRIVATE include)
target_link_libraries(cache_2q_tests PRIVATE GTest::gtest_main)

add_executable(cache_belady_tests tests/cache_belady_tests.cpp)
target_include_directories(cache_belady_tests PRIVATE include)
target_link_libraries(cache_belady_tests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(cache_2q_tests cache_belady_tests)
