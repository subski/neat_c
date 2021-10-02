#include "neurolution/specie.h"

#include "neurolution/agent.h"

#include "data_structures/clist.h"

void specie_sort(Specie* specie, int order)
{
    clist* ordered = NULL;;

    clist* tmp;
    CY_ITER_DATA(specie->specimens, agent_node, agent, Agent*,;
        if (ordered == NULL)
        {
            cy_insert(&ordered, agent);            
        }
        else
        {
            CY_ITER_DATA(ordered, agentO_node, agentO, Agent*, 
                if (order == 1 && agent->fitness < agentO->fitness || order == -1 && agent->fitness > agentO->fitness)
                {
                    tmp = malloc(sizeof(clist));
                    tmp->data = agentO;
                    agentO_node->data = agent;
                    tmp->next = agentO_node->next;
                    agentO_node->next = tmp;

                    break;
                }
                else if (agentO_node->next == ordered)
                {
                    tmp = malloc(sizeof(clist));
                    tmp->data = agent;
                    tmp->next = ordered;
                    agentO_node->next = tmp;
                    break;
                }
            );
        }
    );

    cy_clear(&specie->specimens);
    specie->specimens = ordered;
}

void free_specie(Specie* specie)
{
    if (specie->centroid != NULL)
	    free_agent(&specie->centroid);
    if (specie->specimens != NULL)
	    cy_clear(&specie->specimens);
}