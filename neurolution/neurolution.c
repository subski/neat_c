#include "neurolution/neurolution.h"

#include "data_structures/pool.h"
#include "data_structures/clist.h"
#include "data_structures/vector.h"

#include "neurolution/agent.h"
#include "neurolution/mutations.h"
#include "neurolution/kmeans.h"
#include "neurolution/env_settings.h"

#include "tools/ui.h"


pool* P_AGENT  = NULL;
pool* P_NEURON = NULL;
pool* P_LINK   = NULL;

uint32_t NeuronCount = INPUT_SIZE + OUTPUT_SIZE;

NeuronHistory_s NeuronHistory = { 0, 0, NULL };

clist* Population = NULL;
clist* Species = NULL;

void evolve(void)
{
#if USE_SDL
	// ui_init();
	// ui_run();
#endif // !USE_SDL
	// TODO: create offsprings before killing agents
	createInitialPopulation(&Population, MAX_POPULATION);
	
	// speciate
	kmeans_init(Population, &Species, 3);
	kmeans_run(Population, Species);

	for (int step = 1; step <= 1; step++)
	{
		// fitness sharing
		double agent_count = 1.0 / cy_len(Population);
		CY_ITER_DATA(Species, specie_node, specie, Specie*,
			specie->fitness = 0.0;
			CY_ITER_DATA(specie->specimens, agent_node, agent, Agent*,
				specie->fitness += agent->fitness;
			);
			specie->proportion = specie->fitness * agent_count * MAX_POPULATION * 0.5;
		);

		// killing agents
		int dead_count = 0;
		Agent* dead_agent;
		clist* specie_node = Species;
		Specie* specie;
		do
		{
			specie = (Specie*) specie_node->data;
			printf("Killing specie: %d -> %lf\n", specie->id, specie->proportion);
			for (int i=0; i < (int) specie->proportion; i++)
			{
				dead_count++;
				dead_agent = specie->specimens->data;
				
				cy_remove(&Population, dead_agent);
				cy_remove(&specie->specimens, dead_agent);
				free_agent(&dead_agent);
			}
			next(specie_node);
		} while (specie_node != Species);

		printf("dead: %d\n", dead_count);

		// get centroid of elite agents
		kmeans_run(Population, Species);

		//T create offsprings
		//T add offsprings to pop


		//T add offsprings in correct specie
		// speciate entire pop
		kmeans_run(Population, Species);

		//T Eval Fitness
	}

}

void createInitialPopulation(clist** population, uint32_t count)
{
	for (uint32_t i = 0; i < count; i++)
	{
		cy_insert(population, new_BasicAgent(INPUT_SIZE, OUTPUT_SIZE));
	}
}

void free_specie(Specie* specie)
{
	free_agent(&specie->centroid);
	cy_clear(&specie->specimens);
}

void free_neurolution()
{
	CY_ITER_DATA(Species, specie_node, specie, Specie*,
		free_specie(specie);
	);

	cy_clean(&Species);


	// Free the agents
	CY_ITER_DATA(Population, agent_node, agent, Agent*, 
		free_agent(&agent);
	);
	cy_clear(&Population);

	// Free memory pools
	cy_clean(&P_LINK);
	cy_clean(&P_NEURON);
	cy_clean(&P_AGENT);

	// Free neuron history
	NeuronHistory_s* node = NeuronHistory.next;
	NeuronHistory_s* tmp;

	while (node)
	{
		tmp = node->next;
		free(node);
		node = tmp;
	}
}
