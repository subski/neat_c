#include "neurolution/agent.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "data_structures/clist.h"
#include "data_structures/pool.h"
#include "neurolution/neuron.h"
#include "neurolution/neurolution.h"

Agent* createBasicAgent()
{
	Agent* new_agent = request(&P_AGENT, sizeof(Agent));

	new_agent->fitness = 0.0;
	new_agent->neurons = NULL;
	new_agent->links = NULL;

	// create input neurons
	for (int i = 0; i < INPUT_SIZE; i++)
	{
		Neuron* input_neuron = request(&P_NEURON, sizeof(Neuron));

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
	for (int i = 0; i < OUTPUT_SIZE; i++)
	{
		Neuron* output_neuron = request(&P_NEURON, sizeof(Neuron));

		output_neuron->id = INPUT_SIZE + i + 1;
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
	for (int i = 0; i < OUTPUT_SIZE; i++)
	{
		for (int j = 0; j < INPUT_SIZE; j++)
		{
			Link* new_link = request(&P_LINK, sizeof(Link));

			new_link->source = new_agent->inputNeurons[j];
			new_link->weight = 1.0;
			new_link->activated = true;

			insert(&new_agent->outputNeurons[i]->links, new_link);
			insert(&new_agent->links, new_link);
		}
	}

	return new_agent;
}

void free_agent(Agent** agent)
{
	pclean(&(*agent)->links, &P_LINK);
	ITER((*agent)->neurons, neuron_node,
		clear(&((Neuron*)neuron_node->data)->links);
	);
		pclean(&(*agent)->neurons, &P_NEURON);
	pfree(&P_AGENT, agent, 0);
}

// TODO: use macros instead.

void print_agent(Agent* agent)
{
	printf("agent:\n");
	clist* neuron_node = agent->neurons;
	Neuron* neuron;
	do
	{
		neuron = (Neuron*)neuron_node->data;

		if (neuron->links == NULL)
		{
			next(neuron_node);
			continue;
		}
		clist* link_node = neuron->links;
		Link* link;
		do
		{
			link = (Neuron*)link_node->data;
			printf("%d->%d\n", link->source->id, neuron->id);
			next(link_node);
		} while (link_node != neuron->links);

		next(neuron_node);
	} while (neuron_node != agent->neurons);

	printf("%d\n\n", len(agent->links));
}