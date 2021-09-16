cmake_minimum_required(VERSION 3.10.0)

project(NEAT_C C)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(SOURCES
    ${PROJECT_SOURCE_DIR}/main.c
    ${PROJECT_SOURCE_DIR}/pch.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(neurolution)
add_subdirectory(data_structures)
add_subdirectory(tools)

target_include_directories(${PROJECT_NAME} PRIVATE ./)

target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)

# Link the math library, needed on UNIX systems.
if (UNIX)
    target_link_libraries(${PROJECT_NAME} m)
endif (UNIX)

# Add folder filters in Visual Studio projects.
if (WIN32)
    get_target_property(sources ${PROJECT_NAME} SOURCES)
    foreach(file ${sources})
        get_filename_component(path "${file}" ABSOLUTE)
        get_filename_component(path "${path}" PATH)
        if(file MATCHES "${PROJECT_BINARY_DIR}")
            file(RELATIVE_PATH path ${PROJECT_BINARY_DIR} "${path}")
        else()
            file(RELATIVE_PATH path ${PROJECT_SOURCE_DIR} "${path}")
        endif()
        string(REGEX REPLACE "/" "\\\\" win_path "${path}")
        source_group("${win_path}" REGULAR_EXPRESSION "${path}/[^/\\]+\\..*")
    endforeach()
endif (WIN32)

set(CMAKE_SUPPRESS_REGENERATION true)