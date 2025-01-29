cmake_minimum_required(VERSION 3.21)

project (aoc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_DEPENDS_USE_COMPILER FALSE)

add_subdirectory(cmake EXCLUDE_FROM_ALL)

enable_testing()

add_subdirectory(2022)
add_subdirectory(2023)
add_subdirectory(2024)
