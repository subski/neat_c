cmake_minimum_required(VERSION 3.10.0)

project(NEAT_C C)

set(SOURCES
    ${PROJECT_SOURCE_DIR}/main.c
    ${PROJECT_SOURCE_DIR}/pch.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(neurolution)
add_subdirectory(data_structures)
add_subdirectory(tools)

if (UNIX)
    target_link_libraries(${PROJECT_NAME} m)
endif (UNIX)

target_include_directories(${PROJECT_NAME} PRIVATE ./)

target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)

set(CMAKE_SUPPRESS_REGENERATION true)