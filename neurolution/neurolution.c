#include "neurolution/neurolution.h"

#include <math.h>

#include "data_structures/pool.h"
#include "data_structures/clist.h"
#include "data_structures/vector.h"

#include "neurolution/Agent.h"
#include "neurolution/env_settings.h"

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

void idToPair(uint32_t id, uint32_t* p1, uint32_t* p2)
{
	uint32_t r_id;

	if (id % 2 == 0)
	{
		r_id = id / 2;
	}
	else
	{
		r_id = (id + 1) / 2;
	}

	float n = 0.5f + sqrtf(0.25f + 2 * r_id);
	float nb = ceilf(n) - 1;

	if (id % 2 == 0)
	{
		*p1 = (int)roundf(nb * (n - nb));
		*p2 = (int)ceilf(n);
	}
	else
	{
		*p1 = (int)ceilf(n);
		*p2 = (int)roundf(nb * (n - nb));
	}
}


// TODO: Simplify equation.
uint32_t pairToId(uint32_t p1, uint32_t p2)
{
	uint32_t tmp = 0;
	if (p1 < p2)
	{
		tmp = p1;
		p1 = p2;
		p2 = tmp;
	}

	uint32_t N = p1 * (p1 - 1) / 2;
	uint32_t N2 = (p1-1) * (p1 - 2) / 2;
	
	uint32_t id = (int)N2 + (((float)p2 / p1) * (N - N2));
	id = id*2+1;

	if (tmp)
		id++;

	return id;
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
