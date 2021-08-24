#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

#include <time.h>

#include <Windows.h>

#include "utils.h"
#include "env_settings.h"
#include "clist.h"
#include "llist.h"
#include "pool.h"
#include "neuron.h"
#include "agent.h"
#include "neurolution.h"


Agent* cBA(void)
{

	Agent* new_agent = malloc(sizeof(Agent));

	new_agent->fitness = 0.0;
	new_agent->neurons = NULL;
	new_agent->links = NULL;

	// create input neurons
	for (int i = 0; i < 3; i++)
	{
		Neuron* input_neuron = malloc(sizeof(Neuron));

		input_neuron->id = i + 1;
		input_neuron->enabled = true;
		input_neuron->activated = false;
		input_neuron->type = INPUT_TYPE;
		input_neuron->activationFunc = NULL;
		input_neuron->value = 0.0;
		input_neuron->bias = 0.0;
		input_neuron->links = NULL;

		new_agent->inputNeurons[i] = input_neuron;
		insert(&new_agent->neurons, input_neuron);
	}

	// create output neurons
	for (int i = 0; i < 2; i++)
	{
		Neuron* output_neuron = malloc(sizeof(Neuron));

		output_neuron->id = 3 + i + 1;
		output_neuron->enabled = true;
		output_neuron->activated = false;
		output_neuron->type = OUTPUT_TYPE;
		output_neuron->activationFunc = NULL;
		output_neuron->value = 0.0;
		output_neuron->bias = 0.0;
		output_neuron->links = NULL;

		new_agent->outputNeurons[i] = output_neuron;
		insert(&new_agent->neurons, output_neuron);
	}

	// create links
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Link* new_link = malloc(sizeof(Link));

			new_link->source = new_agent->inputNeurons[j];
			new_link->weight = 1.0;
			new_link->activated = true;

			insert(&new_agent->outputNeurons[i]->links, new_link);
			insert(&new_agent->links, new_link);
		}
	}

	return new_agent;
}

int main(void)
{

	clist* agents = NULL;
	clist* tmp;
	clock_t start_time;




	_sleep(3000);

	start_time = clock();

	for (int k = 0; k < 100; k++)
	{


		for (int i = 0; i < 20000; i++)
		{
			insert(&agents, createBasicAgent());
		}

		for (int i = 0; i < 19990; i++)
		{
			tmp = agents->next;
			agents->next = tmp->next;
			free_agent(&tmp->data);
			free(tmp);
		}


	}

	printf("%d\n", clock() - start_time);

	clist* iter = agents;
	do
	{
		free_agent((Agent**)&iter->data);
		next(iter);
	} while (iter != agents);

	clear(&agents);

	clean(&P_LINK);
	clean(&P_NEURON);
	clean(&P_AGENT);


	_sleep(3000);

	
	start_time = clock();

	for (int k = 0; k < 100; k++)
	{

		for (int i = 0; i < 20000; i++)
		{
			insert(&agents, cBA());
		}

		for (int i = 0; i < 19990; i++)
		{
			tmp = agents->next;
			agents->next = tmp->next;

			Agent* agent = tmp->data;
			clean(&agent->links);
			CLIST_ITER(agent->neurons, neuron_node,
				clear(&((Neuron*)neuron_node->data)->links);
			)
			clean(&agent->neurons);
			free(agent);


			free(tmp);
		}


	}
	printf("%d\n", clock() - start_time);

	CLIST_ITER(agents, agent_node,
		clean(&((Agent*)agents->data)->links);
		clean(&((Agent*)agents->data)->neurons);
	)
	clean(&agents);

	system("pause");
	return EXIT_SUCCESS;
}