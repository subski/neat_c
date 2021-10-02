#include "neurolution/neurolution.h"

#include <math.h>

#include "data_structures/pool.h"
#include "data_structures/clist.h"
#include "data_structures/vector.h"

#include "neurolution/agent.h"
#include "neurolution/specie.h"
#include "neurolution/mutations.h"
#include "neurolution/kmeans.h"
#include "neurolution/env_settings.h"

#include "tools/ui.h"
#include "tools/utils.h"


pool* P_AGENT  = NULL;
pool* P_NEURON = NULL;
pool* P_LINK   = NULL;

uint32_t NeuronCount = INPUT_SIZE + OUTPUT_SIZE;

NeuronHistory_s NeuronHistory = { 0, 0, NULL };

Generation CurrentGeneration = { NULL, NULL };
Generation NextGeneration = { NULL, NULL };

const float elite_percentage = 0.5f;

void evolve(void)
{
	// initial population
	createInitialPopulation(&CurrentGeneration.Population, MAX_POPULATION);

	// initial speciation
	kmeans_init(CurrentGeneration.Population, &CurrentGeneration.Species, 3);
	kmeans_run(CurrentGeneration.Population, CurrentGeneration.Species);

	// generations
	for (int step = 1; step <= 1; step++)
	{
		NextGeneration = (Generation) { NULL, NULL };

		// TodoStep: eval fitness of the population
		
		fitnessSharing(CurrentGeneration.Species);

		produceNextGeneration(&CurrentGeneration, &NextGeneration);

		// speciation
		kmeans_run(NextGeneration.Population, NextGeneration.Species);

		_advanceGeneration();
	}
}

void createInitialPopulation(clist** population, uint32_t count)
{
	for (uint32_t i = 0; i < count; i++)
	{
		Agent* agent = new_BasicAgent(INPUT_SIZE, OUTPUT_SIZE);
		agent->fitness = i;
		cy_insert(population, agent);
	}
}

void fitnessSharing(clist* species)
{

	//double proportion_leftover = 0;
	// _fitness sharing
	// the fitness of a specie is the sum of the fitness of his agent divided by the number of agents
	// So a specie cannot afford to become too big
	double totalFitness = 0.0;
	CY_ITER_DATA(species, specie_node, specie, Specie*,
		specie_computeFitness(specie);
		totalFitness += specie->fitness;
	);

	// Proportion is the number of agents the specie is allowed to have in the next generation
	// and is proportionnal on how high the specie's fitness is.
	CY_ITER_DATA(species, specie_node, specie, Specie*,
		double _prop = specie->fitness / totalFitness * MAX_POPULATION;
		//proportion_leftover += _prop - (int)_prop;
		specie->proportion = (int)_prop;
	);

	// TODO: what to do with the leftovers ?
	//printf("Leftover: %lf\n", proportion_leftover);
}

void produceNextGeneration(Generation *_gen, Generation *_nxtGen)
{

	// TODO: if specie only has a couple members -> merge it with the nearest specie

	// Put all the elites to the next generation and mutate them
	clist* specie_node = _gen->Species;
	Specie* currentSpecie, * nextSpecie;
	Agent* elite;
	clist* elite_node;
	int specie_size, nb_elite;
	do
	{
		currentSpecie = (Specie*)specie_node->data;
		nextSpecie = specie_copy(currentSpecie);
		specie_sortByFitness(currentSpecie, -1);
		specie_size = cy_len(currentSpecie->specimens);
		nb_elite = (int)round(currentSpecie->proportion * elite_percentage);
		elite_node = currentSpecie->specimens;
		//printf("nb elite: %d\n", nb_elite);
		for (int i = 0; i < nb_elite; i++)
		{
			elite = (Agent*)elite_node->data;
			elite->survive = true;
			cy_insert(&NextGeneration.Population, elite);
			cy_insert(&nextSpecie->specimens, elite);
			mutate_agent(elite);
			next(elite_node);
		}

		cy_insert(&_nxtGen->Species, nextSpecie);

		next(specie_node);
	} while (specie_node != _gen->Species);

	kmeans_run(_nxtGen->Population, NextGeneration.Species);


	// _create offsprings
	specie_node = _gen->Species;
	do
	{
		currentSpecie = (Specie*)specie_node->data;

		//printf("S %d : nb offsprings: %d\n", currentSpecie->id, (int)round(currentSpecie->proportion * (1.f - elite_percentage)));

		bool asexualOnly = false;
		if (cy_len(currentSpecie->specimens) == 1)
			asexualOnly = true;

		for (int i = 0; i < (int)round(currentSpecie->proportion * (1.f - elite_percentage)); i++)
		{
			Agent* agent1 = NULL, * agent2 = NULL, * offSpring = NULL;

			if (asexualOnly)
			{
				offSpring = agent_clone((Agent*)currentSpecie->specimens->data);
				mutate_agent(offSpring);
			}
			else
			{
				while (agent1 == agent2)
				{
					agent1 = cy_random_data(currentSpecie->specimens);
					agent2 = cy_random_data(currentSpecie->specimens);
				}
				offSpring = crossOver(agent1, agent2);
			}

			cy_insert(&_nxtGen->Population, offSpring);
			species_insert(_nxtGen->Species, offSpring);
		}
		next(specie_node);
	} while (specie_node != _gen->Species);
}


void _advanceGeneration()
{
	CY_ITER_DATA(CurrentGeneration.Population, agent_node, agent, Agent*,
				 if (agent->survive)
				 {
					 agent->survive = false;
				 }
				 else
				 {
					 free_agent(&agent);
				 }
	);
	cy_clear(&CurrentGeneration.Population);

	CY_ITER_DATA(CurrentGeneration.Species, specie_node, specie, Specie*,
				 free_specie(specie);
	);

	cy_clean(&CurrentGeneration.Species);

	CurrentGeneration.Population = NextGeneration.Population;
	CurrentGeneration.Species = NextGeneration.Species;
}

void free_neurolution()
{
	CY_ITER_DATA(CurrentGeneration.Species, specie_node, specie, Specie*,
		free_specie(specie);
	);

	cy_clean(&CurrentGeneration.Species);


	// Free the agents
	CY_ITER_DATA(CurrentGeneration.Population, agent_node, agent, Agent*, 
		free_agent(&agent);
	);
	cy_clear(&CurrentGeneration.Population);

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
