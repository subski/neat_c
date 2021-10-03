#include "neurolution/kmeans.h"

#include <math.h>

#include "neurolution/neurolution.h"
#include "neurolution/agent.h"
#include "neurolution/specie.h"
#include "neurolution/neuron.h"

#include "data_structures/vector.h"
#include "data_structures/clist.h"
#include "data_structures/pool.h"

#include "tools/utils.h"
#include "tools/pcg_basic.h"

// TODO: add documention for KMeans algo

void kmeans_init(clist* datalist, clist** specieslist, int species_count)
{
	Agent* agent_centroid;
	Specie* specie;
    clist *data = cy_clone(datalist);
	for ( int i = 0; i < species_count; i++ )
	{
		agent_centroid = cy_random_data( data );
		specie = malloc( sizeof( Specie ) );
        specie->centroid = agent_clone(agent_centroid);
		specie->specimens = NULL;
		specie->id = i+1;
		cy_insert( &specie->specimens, agent_centroid );
		cy_insert( specieslist, specie );
		cy_remove( &data, agent_centroid );
	}

	speciate(data, *specieslist);

	cy_clear(&data);
}

void kmeans_run(clist* datalist, clist* specieslist)
{
    int changes = 0;
    do
    {
        CY_ITER_DATA(specieslist, specie_node, specie, Specie*,
			CalculateCentroidAgent(specie->specimens, specie->centroid);
		);
        
		// TODO: only recalculate centroid for species that have been modified
        CY_ITER_DATA( specieslist, specie_node, specie, Specie*,
            cy_clear(&specie->specimens);
        );

        changes = speciate(datalist, specieslist);

    } while (changes != 0);
}

int speciate( clist* datalist, clist* species )
{
	int changes = 0;
	CY_ITER_DATA( datalist, agent_node, agent, Agent*,
		changes += species_insert(species, agent);
	);
	
	return changes;
}

void CalculateCentroidAgent(clist* agentList, Agent* centroid_agent)
{
	if (cy_len(agentList) == 0) return;

	cy_pclean(&centroid_agent->linkList, &P_LINK);
	CY_ITER_DATA(centroid_agent->neuronList, neuron_node, neuron, Neuron*,
		 cy_clear(&neuron->linkList);
	);
	cy_pclean(&centroid_agent->neuronList, &P_NEURON);

	double agent_count = 0;
	clist* iter_agent = agentList;
	Agent* agent;
	Neuron *neuron_centroid;
	Link* link_centroid;
	do
	{
		agent = (Agent*) iter_agent->data;

		CY_ITER_DATA(agent->neuronList, neuron_node, neuron_agent, Neuron*,
			neuron_centroid = getNeuronInAgent(centroid_agent, neuron_agent->id);
			if (!neuron_centroid)
			{
				neuron_centroid = new_Neuron(
					neuron_agent->id, 
					neuron_agent->enabled, 
					neuron_agent->activated, 
					neuron_agent->type, 
					neuron_agent->activationFunc, 
					neuron_agent->value,
					neuron_agent->bias,
					NULL);
				cy_insert(&centroid_agent->neuronList, neuron_centroid);
			}

			// We iterate through all the links of both neurons and we chose at random links from the neuron1 links.
			CY_ITER_DATA(neuron_agent->linkList, link_node, link_agent, Link*,
				link_centroid = getLinkInNeuron(neuron_centroid, link_agent->id);
				if (link_centroid == NULL)
				{
					link_centroid = cy_insert(&neuron_centroid->linkList,
						new_LinkId(
							link_agent->source->id, 
							link_agent->target->id, 
							link_agent->weight, 
							link_agent->enabled));
				}
				else
				{
					link_centroid->weight += link_agent->weight;
				}
			);
		);
		agent_count++;
		next(iter_agent);
	} while (iter_agent != agentList);

	// Actually initialize the links in the child. There is probably a better way.
    agent_count = 1 / agent_count;
	uint32_t p1, p2;
    CY_ITER_DATA(centroid_agent->neuronList, neuron_node, neuron, Neuron*,
        CY_ITER_DATA(neuron->linkList, link_node, link, Link*,
            cy_insert(&centroid_agent->linkList, link);
            idToPair(link->id, &p1, &p2);
            link->source = getNeuronInAgent(centroid_agent, p1);
            link->target = neuron;
			link->weight *= agent_count;
        );
    );
}