
# Get the ProjectBits code
set(project "projectbits")
FetchContent_Declare(
    ${project}
    GIT_REPOSITORY "https://github.com/ilya1725/ProjectBits"
    GIT_PROGRESS   true
)

FetchContent_GetProperties(${project})
if(NOT ${project}_POPULATED)
    set(FETCHCONTENT_QUIET OFF)
    FetchContent_Populate(${project})
    add_subdirectory(${${project}_SOURCE_DIR} ${${project}_BINARY_DIR} EXCLUDE_FROM_ALL)
    SET(PROJECTBITS_PATH "${${project}_SOURCE_DIR}" CACHE INTERNAL "ProjectBits path")
endif()