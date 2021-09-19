set(SOURCES
    neurolution.h
    neurolution.c

    agent.h
    agent.c

    agent_io.h
    agent_io.c

    neuron.h
    neuron.c

    mutations.h
    mutations.c

    env_settings.h
)

list(TRANSFORM SOURCES PREPEND ${CMAKE_CURRENT_LIST_DIR}/)

target_sources(${PROJECT_NAME}
    PRIVATE 
    ${SOURCES}
)