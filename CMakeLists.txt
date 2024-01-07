cmake_minimum_required(VERSION 3.20)

project (ProjectBits)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(cmake EXCLUDE_FROM_ALL)

enable_testing()

add_subdirectory(2022)
add_subdirectory(2023)

