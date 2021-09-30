#include "neurolution/neurolution.h"

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

void free_neurolution()
{
	// Free the agents
	for (uint32_t i = 0; i < Population.count; i++)
	{
		free_agent(&VEC(Population, Agent*, i));
	}

	free_vector(&Population);


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
