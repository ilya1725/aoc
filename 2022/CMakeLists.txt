set(year-prefix 2022)

file(GLOB files "*.cpp")
foreach(file ${files})
    get_filename_component(file-prefix ${file} NAME_WLE)

    add_executable(${year-prefix}-${file-prefix} ${file})
endforeach()