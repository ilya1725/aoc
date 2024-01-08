set(year-prefix 2023)

file(GLOB files "*.cpp")
foreach(file ${files})
    get_filename_component(file-prefix ${file} NAME_WLE)

    add_executable(${year-prefix}-${file-prefix} ${file})

    target_include_directories(${year-prefix}-${file-prefix} PRIVATE ${PROJECTBITS_PATH}/doodles/src)

    SET(BUILD_MODE Debug)
    SET(CMAKE_CXX_FLAGS "-Wall")
    SET(CMAKE_CXX_FLAGS "-g")

endforeach()