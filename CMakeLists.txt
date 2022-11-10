cmake_minimum_required(VERSION 3.16)

project (ProjectBits)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(cmake EXCLUDE_FROM_ALL)

enable_testing()

add_subdirectory(tlgr)
add_subdirectory(gtest)
