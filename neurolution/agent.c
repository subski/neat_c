#include "neurolution/agent.h"

#include "data_structures/clist.h"
#include "data_structures/pool.h"
#include "data_structures/vector.h"

#include "neurolution/neuron.h"
#include "neurolution/neurolution.h"

Agent* new_BasicAgent(uint32_t inputCount, uint32_t outputCount)
{
	Agent* new_agent = request(&P_AGENT, sizeof(Agent));

	new_agent->inputVector = new_vector(sizeof(Neuron*), inputCount, 0);
	new_agent->outputVector = new_vector(sizeof(Neuron*), outputCount, 0);

	new_agent->fitness = 0.0;
	new_agent->neuronList = NULL;
	new_agent->linkList = NULL;

	// create input neurons
	for (uint32_t i = 0; i < inputCount; i++)
	{
		Neuron* input_neuron = new_BasicNeuron(i + 1);

		input_neuron->type = INPUT_TYPE;

		vec_insert(&new_agent->inputVector, &input_neuron, i);
		insert(&new_agent->neuronList, input_neuron);
	}

	// create output neurons
	for (uint32_t i = 0; i < outputCount; i++)
	{
		Neuron* output_neuron = new_BasicNeuron(inputCount + i + 1);

		output_neuron->type = OUTPUT_TYPE;

		vec_insert(&new_agent->outputVector, &output_neuron, i);
		insert(&new_agent->neuronList, output_neuron);
	}

	// create links
	for (uint32_t i = 0; i < outputCount; i++)
	{
		for (uint32_t j = 0; j < inputCount; j++)
		{
			Link* new_link = new_Link(*(Neuron**)vec_get(&new_agent->inputVector, j), *(Neuron**)vec_get(&new_agent->outputVector, i), 0.0, true);
			insert(&new_agent->linkList, new_link);
		}
	}

	return new_agent;
}

void free_agent(Agent** agent)
{
	pclean(&(*agent)->linkList, &P_LINK);
	ITER((*agent)->neuronList, neuron_node,
		 clear(&((Neuron*)neuron_node->data)->linkList);
	);
	pclean(&(*agent)->neuronList, &P_NEURON);

	free_vector(&(*agent)->inputVector);
	free_vector(&(*agent)->outputVector);

	pfree(&P_AGENT, agent);
}

bool check_agent(Agent* agent)
{
	// Check dupplicate links
	int cpt;
	ITER_V(agent->linkList, link_node, link, Link*,
		cpt = 0;
		ITER_V(agent->linkList, link_node2, link2, Link*,
			if (link->source->id == link2->source->id && link->target->id == link2->target->id)
				cpt++;
		);
		if (cpt != 1)
			return false;
	);
	
	// Check dupplicate neurons
	ITER_V(agent->neuronList, neuron_node, neuron, Neuron*,
		cpt = 0;
		ITER_V(agent->neuronList, neuron_node2, neuron2, Neuron*,
			if (neuron->id == neuron2->id)
				cpt++;
		);
		if (cpt != 1)
			return false;
	);

	// Input neurons are not supposed to have any links attached to them
	for (int i = 0; i < INPUT_SIZE; i++)
	{
		if ((*(Neuron**)vec_get(&agent->inputVector, i))->linkList != NULL)
			return false;
	}


	return true;
}

// TODO: use macros instead.

void print_agent(Agent* agent)
{
	printf("agent:\n");
	ITER_V(agent->neuronList, neuron_node, neuron, Neuron*,
		   ITER_V(neuron->linkList, link_node, link, Link*,
		   printf("%d -> %d [%.2lf, %d]\n", link->source->id, neuron->id, link->weight, link->enabled);
	);	);

	printf("Neurons: %d | Links: %d | Fitness: %lf\n", len(agent->neuronList), len(agent->linkList), agent->fitness);
}