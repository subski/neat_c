#include "neurolution/neurolution.h"

#include <math.h>

#include "data_structures/pool.h"
#include "data_structures/clist.h"
#include "data_structures/vector.h"

#include "neurolution/agent.h"
#include "neurolution/mutations.h"
#include "neurolution/env_settings.h"

#include "tools/ui.h"


pool* P_AGENT  = NULL;
pool* P_NEURON = NULL;
pool* P_LINK   = NULL;

uint32_t NeuronCount = INPUT_SIZE + OUTPUT_SIZE;

NeuronHistory_s NeuronHistory = { 0, 0, NULL };

vector Population;

void evolve(void)
{
	Population = new_vector(sizeof(Agent*), MAX_POPULATION, 0);

	createInitialPopulation(&Population, MAX_POPULATION);
	
	// Speciate
	printf("Speciation.\n");
	
#if USE_SDL
	ui_init();
	ui_exit();
#endif // !USE_SDL

	// Selection
	
	// Reproduction

	// Mutation

	// Calcul Fitness

}

void createInitialPopulation(vector* population, uint32_t count)
{
	for (uint32_t i = 0; i < count; i++)
	{
		Agent* agent = new_BasicAgent(INPUT_SIZE, OUTPUT_SIZE);
		vec_set(population, &agent, i);
	}
}

// TODO: Review the use of calculating pairs id instead of storing them.
// Idea: A [{MAX_NEURONS}x{MAX_NEURONS}] matrix where each element is a {POPULATION_SIZE} array with pointers to links.
void idToPair(int id, uint32_t* p1, uint32_t* p2)
{
	bool invert = false;
	if (id < 0)
	{
		id = -id;
		invert = true;
	}

	#pragma warning (push)
	#pragma warning(disable : 4244)
	float n1 = 0.5f + sqrt(2 * id + 0.25f); // magic formula
	uint32_t n1_c = ceilf(n1);
	
	uint32_t n2 = (n1_c - 1) * (n1 - (n1_c - 1 ));
	#pragma warning (pop)

	if (!invert)
	{
		*p1 = n1_c;
		*p2 = n2;
	}
	else
	{
		*p1 = n2;
		*p2 = n1_c;
	}
}

int pairToId(uint32_t p1, uint32_t p2)
{
	int invert = 1;
	if (p2 > p1)
	{
		invert = -1;
		int tmp = p1;
		p1 = p2;
		p2 = tmp;		
	}

	return invert * ((p1 * (p1 - 1)) / 2 - (p1 - p2 - 1));
}

void free_neurolution()
{
	// Free the agents
	for (uint32_t i = 0; i < Population.count; i++)
	{
		free_agent(vec_get(&Population, i));
	}

	free_vector(&Population);


	// Free memory pools
	clean(&P_LINK);
	clean(&P_NEURON);
	clean(&P_AGENT);

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
