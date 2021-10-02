set(SOURCES 
    XorEvaluator.h
    XorEvaluator.c
)

list(TRANSFORM SOURCES PREPEND ${CMAKE_CURRENT_LIST_DIR}/)

target_sources(${PROJECT_NAME}
    PRIVATE 
    ${SOURCES}
)