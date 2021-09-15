set(SOURCES 
        malloc_dbg.h
        malloc_dbg.c

        pcg_basic.h
        pcg_basic.c

        utils.h
        utils.c

)

list(TRANSFORM SOURCES PREPEND ${CMAKE_CURRENT_LIST_DIR}/)

target_sources(${PROJECT_NAME}
    PRIVATE 
    ${SOURCES}
)