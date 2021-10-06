#include "neurolution/neurolution.h"

#include <math.h>

#include "data_structures/pool.h"
#include "data_structures/clist.h"
#include "data_structures/vector.h"

#include "neurolution/agent.h"
#include "neurolution/agent_io.h"
#include "neurolution/specie.h"
#include "neurolution/neuron.h"
#include "neurolution/mutations.h"
#include "neurolution/kmeans.h"
#include "neurolution/env_settings.h"

#include "tasks/XorEvaluator.h"

#include "tools/ui.h"
#include "tools/utils.h"

#include "CPPN/activations.h"

pool* P_AGENT  = NULL;
pool* P_NEURON = NULL;
pool* P_LINK   = NULL;

double (*fitneseEvaluator)(Agent*);

uint32_t NeuronCount = INPUT_SIZE + OUTPUT_SIZE;

NeuronHistory_s NeuronHistory = { 0, 0, NULL };

Generation CurrentGeneration = { NULL, NULL };
Generation NextGeneration = { NULL, NULL };

void evolve(double fitness_goal)
{
	Agent* top_agent;
	fitneseEvaluator = &XorEvaluator;

	// initial population
	createInitialPopulation(&CurrentGeneration.Population, MAX_POPULATION);

	// initial speciation
	kmeans_init(CurrentGeneration.Population, &CurrentGeneration.Species, SPECIE_COUNT);

	kmeans_run(CurrentGeneration.Population, CurrentGeneration.Species);
	
	char pid[255] = { '\0' };
	// generations
	int step = 0;
	do
	{
		step++;
		NextGeneration = (Generation) { NULL, NULL };

		top_agent = population_eval(CurrentGeneration.Population);
		
		fitnessSharing(CurrentGeneration.Species);

		produceNextGeneration(&CurrentGeneration, &NextGeneration);

		if (NextGeneration.Population == NULL)
			printf("ERROR\n");

		kmeans_run(NextGeneration.Population, NextGeneration.Species);

		_advanceGeneration();
		
		CY_ITER_DATA(CurrentGeneration.Population, agent_node, agent, Agent*,
			if (check_agent(agent) != 0)
				return;
		);

		printf("Gen %d: species=%d fitness=%.2lf pop=%d ncount=%d \n", 
			step,
			cy_len(CurrentGeneration.Species),
			top_agent->fitness,
			cy_len(CurrentGeneration.Population),
			NeuronCount
		);

		// plot_close(pid);
		// plot_agent(top_agent, pid);

		// _sleep(1000);

	} while ( top_agent->fitness < fitness_goal || step <= 100 );

	printf("fitness=%lf\n", XorEvaluator(top_agent));


	double fitness = 0.0;
    vector* outputVector;
    double output;
	
    for (double i = 0; i < 1.0; i += 0.1)
    {
        outputVector = agent_eval(top_agent, &i, 10);
        output = VEC((*outputVector), Neuron*, 0)->value;
        printf("x:%.1lf o:%.2lf => %.2lf\n", i, output, 1.0 - double_abs(i - output*output));
	}

    // for (int i = 0; i < 4; i++)
    // {
    //     outputVector = agent_eval(top_agent, DATASET[i], 50);
    //     output = VEC((*outputVector), Neuron*, 0)->value;
    //     output = min(max(output, 0.0), 1.0);
		
	// 	printf("In: %.2lf %.2lf %.2lf -> %.2lf (expected: %.2lf) ", DATASET[i][0], DATASET[i][1], DATASET[i][2], output, DATASET[i][3]);

    //     if (DATASET[i][3] == 1)
    //     {
    //         printf("fit: %.2lf \n", 1.0 - ((1.0 - output) * (1.0 - output)));
    //     }
    //     else
    //     {
    //         printf("fit: %.2lf \n", 1.0 - (output * output));
    //     }
    // }

	// CY_ITER_DATA(CurrentGeneration.Species, specie_node, specie, Specie*, 
	// 	show_agent(specie_topAgent(specie));
	// );
	show_agent(top_agent);

	
}

void createInitialPopulation(clist** population, uint32_t count)
{
	for (uint32_t i = 0; i < count; i++)
	{
		Agent* agent = new_BasicAgent(INPUT_SIZE, OUTPUT_SIZE, &ACT_FUNC);
		cy_insert(population, agent);
	}
}

Agent* population_eval(clist* agents)
{
	Agent* top_agent = NULL;
	CY_ITER_DATA(agents, agent_node, agent, Agent*,
		agent->fitness = fitneseEvaluator(agent);
		if (top_agent == NULL || agent->fitness > top_agent->fitness)
		{
			top_agent = agent;
		}
	);

	if (check_agent(top_agent) != 0)
		printf("ERROR\n");

	return top_agent;
}


void fitnessSharing(clist* species)
{
	//double proportion_leftover = 0;
	
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
	// TODO: if specie only has a couple members -> merge agents to nearest specie

	// Put all the elites to the next generation and mutate them
	clist* specie_node, * elite_node;
	Specie* currentSpecie, * nextSpecie;
	Agent* elite, * parent1, * parent2, * offSpring;
	bool asexualOnly;

	// ELITES
	specie_node = _gen->Species;
	do
	{
		currentSpecie = (Specie*)specie_node->data;
		nextSpecie = specie_copy(currentSpecie);
		specie_sortByFitness(currentSpecie, -1);
		

		elite_node = currentSpecie->specimens;
		int elite_count = (int) ceil(currentSpecie->proportion * ELITE_PROP);
		int specimen_count = cy_len(currentSpecie->specimens);
		elite_count = MIN(elite_count, specimen_count);

		for (int i = 0; i < elite_count; i++)
		{
			elite = (Agent*)elite_node->data;
			elite->survive = true;

			cy_insert(&_nxtGen->Population, elite);
			cy_insert(&nextSpecie->specimens, elite);

			mutate_agent(elite);

			next(elite_node);
		}

		
		if (nextSpecie->specimens == NULL)
			printf("ERROR\n");
			
		cy_insert(&_nxtGen->Species, nextSpecie);
			
		next(specie_node);
	} while (specie_node != _gen->Species);

	if (_nxtGen->Population == NULL)
		printf("ERROR\n");

	// kmeans_run(_nxtGen->Population, _nxtGen->Species);
	
	specie_node = _nxtGen->Species;
	do
	{
		Specie* ss = ((Specie*)specie_node->data);
		if (ss->specimens == NULL)
			printf("ERROR\n");
		next(specie_node);
	} while (specie_node != _nxtGen->Species);

	// OFFSPRINGS
	specie_node = _gen->Species;
	do
	{
		currentSpecie = (Specie*)specie_node->data;
		CY_ITER_DATA(_nxtGen->Species, s_node, s, Specie*,
			if (s->id == currentSpecie->id)
			{
				nextSpecie = s;
				break;
			}
		);
		
		if (nextSpecie->specimens == NULL)
			printf("ERROR\n");

		if (cy_len(currentSpecie->specimens) == 1 && !ELITE_OFFSPRING || cy_len(nextSpecie->specimens) == 1 && ELITE_OFFSPRING)
			asexualOnly = true;
		else
			asexualOnly = false;

		for (int i = 0; i < (int) (currentSpecie->proportion * (1.f - ELITE_PROP)); i++)
		{
			parent1 = NULL; parent2 = NULL;
			if (ELITE_OFFSPRING)
			{	
				if (asexualOnly)
				{
					offSpring = agent_clone((Agent*)nextSpecie->specimens->data);
					mutate_agent(offSpring);
				}
				else
				{
					while (parent1 == parent2)
					{
						parent1 = cy_random_data(nextSpecie->specimens);
						parent2 = cy_random_data(nextSpecie->specimens);
					}
					offSpring = crossOver(parent1, parent2);
				}
			}
			else
			{
				if (asexualOnly)
				{
					offSpring = agent_clone((Agent*)currentSpecie->specimens->data);
					mutate_agent(offSpring);
				}
				else
				{
					while (parent1 == parent2)
					{
						parent1 = cy_random_data(currentSpecie->specimens);
						parent2 = cy_random_data(currentSpecie->specimens);
					}
					offSpring = crossOver(parent1, parent2);
				}
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
