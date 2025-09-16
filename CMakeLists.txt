cmake_minimum_required(VERSION 3.11)
project(2q_cache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(cache src/main.cpp)
target_include_directories(cache PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.tar.gz
)

set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(cache_tests tests/cache_tests.cpp)
target_include_directories(cache_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(cache_tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(cache_tests)
