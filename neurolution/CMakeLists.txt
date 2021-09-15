set(SOURCES 
        agent.h
        agent.c

        mutations.h
        mutations.c

        neurolution.h
        neurolution.c

        neuron.h
        neuron.c

        env_settings.h
)

list(TRANSFORM SOURCES PREPEND ${CMAKE_CURRENT_LIST_DIR}/)

target_sources(${PROJECT_NAME}
    PRIVATE 
    ${SOURCES}
)