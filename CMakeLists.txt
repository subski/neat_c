cmake_minimum_required(VERSION 3.10.0)

project(NEAT_C C)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_BUILD_TYPE Debug)

# binary folder after building
set(BIN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG  ${BIN_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${BIN_OUTPUT_DIR})

# source code at root
set(SOURCES
    ${PROJECT_SOURCE_DIR}/main.c
    ${PROJECT_SOURCE_DIR}/pch.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

# copy script 'plot.py' in the bin/ dir to be used by the app
configure_file(${PROJECT_SOURCE_DIR}/plot.py ${BIN_OUTPUT_DIR}/plot.py COPYONLY)
# file(COPY ${PROJECT_SOURCE_DIR}/plot.py DESTINATION ${BIN_OUTPUT_DIR})

add_custom_command(
        TARGET ${PROJECT_NAME}  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${PROJECT_SOURCE_DIR}/plot.py
            ${BIN_OUTPUT_DIR}/plot.py)


# sub folders
add_subdirectory(neurolution)
add_subdirectory(data_structures)
add_subdirectory(tasks)
add_subdirectory(tools)
add_subdirectory(CPPN)

# includes
target_include_directories(${PROJECT_NAME} PRIVATE ./)

# precompile header
target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)

# Linux specifics
if (UNIX)
    # Math lib
    target_link_libraries(${PROJECT_NAME} m)
    # add_definitions(-DUSE_SDL=0)
    find_package(SDL2 REQUIRED)
    include_directories(${PROJECT_NAME} 
    ${CMAKE_CURRENT_LIST_DIR}/third_party/include)
    include_directories(${SDL2_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
endif (UNIX)

# Windows specifics
if (WIN32)

    # SDL lib/include path
    add_definitions(-DUSE_SDL=1)
    include_directories(${PROJECT_NAME} 
        ${CMAKE_CURRENT_LIST_DIR}/third_party/include)
    target_link_libraries(${PROJECT_NAME} 
        ${CMAKE_CURRENT_LIST_DIR}/third_party/lib/x64/SDL2.lib
        ${CMAKE_CURRENT_LIST_DIR}/third_party/lib/x64/SDL2main.lib)

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

# disable unsafe functions warnings
# add_compile_options(/w44101)
if (WIN32)
    set(CMAKE_C_FLAGS "/D_CRT_SECURE_NO_WARNINGS /D_CRT_SECURE_NO_DEPRECATE ${CMAKE_C_FLAGS}")
    set(CMAKE_C_FLAGS "/D_CRT_NONSTDC_NO_DEPRECATE /D_SCL_SECURE_NO_DEPRECATE ${CMAKE_C_FLAGS}")
endif (WIN32)

# Keep track of memory leaks
add_definitions(-DUSE_DEBUG_MALLOC=1)
add_definitions(-DAPP_NAME="${PROJECT_NAME}")

# disable ZERO_CHECK project
set(CMAKE_SUPPRESS_REGENERATION true)