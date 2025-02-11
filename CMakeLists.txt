cmake_minimum_required(VERSION 3.21)

# Check operating system
if (UNIX)
    message("Building on UNIX")
    set(CMAKE_C_COMPILER "gcc.exe")
    set(CMAKE_CXX_COMPILER "g++.exe")
else()
    message("Building on WIN32")
    set(CMAKE_C_COMPILER "C:\\tools\\msys64\\mingw64\\bin\\gcc.exe")
    set(CMAKE_CXX_COMPILER "C:\\tools\\msys64\\mingw64\\bin\\g++.exe")
endif()

project (aoc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_DEPENDS_USE_COMPILER FALSE)
set(CMAKE_USE_RELATIVE_PATHS ON)

# Compiler options
if(CMAKE_COMPILER_IS_GNUCXX)
    message("Update for GNU compiler")
    add_compile_options(-ffile-prefix-map=${CMAKE_SOURCE_DIR}=.)
endif()

add_subdirectory(cmake EXCLUDE_FROM_ALL)

# enable_testing()

add_subdirectory(2022)
add_subdirectory(2023)
add_subdirectory(2024)
