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

void evolve(void)
{
#if USE_SDL
	// ui_init();
	// ui_run();
#endif // !USE_SDL
	// TODO: create offsprings before killing agents (elitism reproduction)
	// _Initialize population
	printf("\033[1;33mInitializing 'Population' \033[0m\n\n");
	createInitialPopulation(&CurrentGeneration.Population, MAX_POPULATION);
	
	
	const float elite_percentage = 0.5f;
	clist* deadPopulation;	
	double proportion_leftover;

	// _speciate
	printf("\033[1;33mInitial Speciation \033[0m\n\n");
	kmeans_init(CurrentGeneration.Population, &CurrentGeneration.Species, 3);
	kmeans_run(CurrentGeneration.Population, CurrentGeneration.Species);

	for (int step = 1; step <= 1; step++)
	{
		NextGeneration = (Generation) { NULL, NULL };
		deadPopulation = NULL;
		proportion_leftover = 0.0;

		// T: eval fitness of the population
		
		// _fitness sharing
		// the fitness of a specie is the sum of the fitness of his agent divided by the number of agents
		// So a specie cannot afford to become too big
		printf("\033[1;33mComputing fitness of species \033[0m\n\n");
		double totalFitness = 0.0;
		CY_ITER_DATA(CurrentGeneration.Species, specie_node, specie, Specie*,
			specie_computeFitness(specie);
			totalFitness += specie->fitness;
		);

		// Proportion is the number of agents the specie is allowed to have in the next generation
		// and is proportionnal on how high the specie's fitness is.
		CY_ITER_DATA(CurrentGeneration.Species, specie_node, specie, Specie*,
			double _prop = specie->fitness / totalFitness * MAX_POPULATION;
			proportion_leftover += _prop - (int)_prop;
			specie->proportion = (int)_prop;
		);

		// TODO: if specie only has a couple members -> merge it with the nearest specie
		
		printf("\033[1;33mSelection of the elite based on fitness \033[0m\n\n");
		clist* specie_node = CurrentGeneration.Species;
		Specie* specie, * nextSpecie;
		Agent* elite;
		clist* elite_node;
		int specie_size, nb_elite;
		do
		{
			specie = (Specie*) specie_node->data;
			nextSpecie = specie_copy(specie);
			specie_sortByFitness(specie, -1);
			specie_size = cy_len(specie->specimens);
			nb_elite = (int) round(specie->proportion * elite_percentage);
			elite_node = specie->specimens;
			for (int i=0; i < nb_elite; i++)
			{
				elite = (Agent*) elite_node->data;
				elite->survive = true;
				cy_insert(&NextGeneration.Population, elite);
				cy_insert(&nextSpecie->specimens, elite);
        		mutate_agent(elite);
				next(elite_node);
			}
			cy_insert(&NextGeneration.Species, nextSpecie);

			next(specie_node);
		} while (specie_node != CurrentGeneration.Species);

		kmeans_run(NextGeneration.Population, NextGeneration.Species);


		//T create offsprings

		// TODO: what to do with the leftovers ?
		printf("Leftover: %lf\n", proportion_leftover);


		//T add offsprings in correct specie
		// _speciate entire pop
		// kmeans_run(Population, Species);

		//T Eval Fitness

		// _free agents
		printf("\033[1;33mRemoving dead agents and moving to the next gen\033[0m\n\n");
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
