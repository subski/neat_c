#include "neurolution/specie.h"

#include "neurolution/agent.h"
#include "neurolution/mutations.h"

#include "data_structures/clist.h"

int species_insert(clist* species, Agent* agent)
{
    int changes = 0;
    Specie* nearest_specie;
    double nearest_distance = -1;
    double target_specie_distance;
    CY_ITER_DATA(species, specie_node, specie, Specie*,
        
        target_specie_distance = agent_euclidean_distance( specie->centroid, agent);
        if ( target_specie_distance < nearest_distance || nearest_distance == -1 )
        {
            nearest_specie = specie;
            nearest_distance = target_specie_distance;
        }
    );
    if (agent->specie != nearest_specie->id)
    {
        changes = 1;
        agent->specie = nearest_specie->id;
    }
    cy_insert(&nearest_specie->specimens, agent);
    return changes;
}

void specie_sortByFitness(Specie* specie, int order)
{
    clist* ordered = NULL;

    clist* tmp;
    CY_ITER_DATA(specie->specimens, agent_node, agent, Agent*,
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

void specie_computeFitness(Specie* specie)
{
    specie->fitness = 0.0;
    CY_ITER_DATA(specie->specimens, agent_node, agent, Agent*,
        specie->fitness += agent->fitness;
    );
    specie->fitness /= cy_len(specie->specimens);
}

void specie_mutate(Specie* specie)
{
    CY_ITER_DATA(specie->specimens, agent_node, agent, Agent*,
        mutate_agent(agent);
    );
}

Agent* specie_topAgent(Specie* specie)
{
    Agent* top_agent;
    double top_fit = 0.0;
    CY_ITER_DATA(specie->specimens, agent_node, agent, Agent*,
        if (agent->fitness > top_fit)
            top_agent = agent;
    );
    return top_agent;
}

Specie* specie_copy(Specie* specie)
{
    Specie* copy = malloc(sizeof(Specie));
    copy->id = specie->id;
    copy->centroid = agent_clone(specie->centroid);
    copy->specimens = NULL;
    copy->fitness = specie->fitness;
    copy->proportion = specie->proportion;
    return copy;
}

void print_specie(Specie* specie)
{
    printf("Specie %d (%d) -> fit : %lf | prop : %d\n", specie->id, cy_len(specie->specimens), specie->fitness, specie->proportion);
}


void print_species(clist* species)
{
    CY_ITER_DATA(species, specie_node, specie, Specie*,
                 print_specie(specie);
                 );
}

void free_specie(Specie* specie)
{
    if (specie->centroid != NULL)
	    free_agent(&specie->centroid);
    if (specie->specimens != NULL)
	    cy_clear(&specie->specimens);
}