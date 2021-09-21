#include "neurolution/agent.h"

#include "data_structures/clist.h"
#include "data_structures/pool.h"
#include "data_structures/vector.h"

#include "neurolution/neuron.h"
#include "neurolution/neurolution.h"

#include "tools/utils.h"

Agent* new_Agent(uint32_t inputSize, uint32_t outputSize)
{
	Agent* new_agent = request(&P_AGENT, sizeof(Agent));

	new_agent->fitness = 0.0;
	new_agent->neuronList = NULL;
	new_agent->linkList = NULL;
	new_agent->inputVector = new_vector(sizeof(Neuron*), inputSize, 0);
	new_agent->outputVector = new_vector(sizeof(Neuron*), outputSize, 0);

	return new_agent;
}

Agent* new_BasicAgent(uint32_t inputSize, uint32_t outputSize)
{
	Agent* new_agent = request(&P_AGENT, sizeof(Agent));

	new_agent->inputVector = new_vector(sizeof(Neuron*), inputSize, 0);
	new_agent->outputVector = new_vector(sizeof(Neuron*), outputSize, 0);

	new_agent->fitness = 0.0;
	new_agent->neuronList = NULL;
	new_agent->linkList = NULL;

	// create input neurons
	for (uint32_t i = 0; i < inputSize; i++)
	{
		Neuron* input_neuron = new_BasicNeuron(i + 1);

		input_neuron->type = INPUT_TYPE;

		vec_set(&new_agent->inputVector, &input_neuron, i);
		insert(&new_agent->neuronList, input_neuron);
	}

	// create output neurons
	for (uint32_t i = 0; i < outputSize; i++)
	{
		Neuron* output_neuron = new_BasicNeuron(inputSize + i + 1);

		output_neuron->type = OUTPUT_TYPE;

		vec_set(&new_agent->outputVector, &output_neuron, i);
		insert(&new_agent->neuronList, output_neuron);
	}

	// create links
	for (uint32_t i = 0; i < outputSize; i++)
	{
		for (uint32_t j = 0; j < inputSize; j++)
		{
			// Link* new_link = new_Link(
			// 	*(Neuron**)vec_get(&new_agent->inputVector, j), 
			// 	*(Neuron**)vec_get(&new_agent->outputVector, i),
			// 	0.0, true);
            Link* new_link = new_Link(
				((Neuron**)new_agent->inputVector.start)[j],
				((Neuron**)new_agent->outputVector.start)[i],
				0.0, true);
			insert(&new_agent->linkList, new_link);
		}
	}

	return new_agent;
}

// TODO: use ++ ont totalNeuron value so if its 2: both neurons are present (it saves an if check)
double distance(Agent* agent1, Agent* agent2, double c1, double c2)
{
	double matching = 0;
	double disjoint = 0;
	double total;
	double weight_diff = 0;

	// Create an array of 'int' the size of NeuronCount
	// where a '1' indicate that a neuron exist in either agent
	// and a '0' means no neurons.
	vector totalNeuron = new_vector(sizeof(int), NeuronCount, 0);
	memset(totalNeuron.start, 0, totalNeuron.type_size * totalNeuron.count);

	ITER_V(agent1->neuronList, neuron_node, neuron, Neuron*,
		vec_set(&totalNeuron, &ONE, neuron->id - 1);
	);
	ITER_V(agent2->neuronList, neuron_node, neuron, Neuron*,
		vec_set(&totalNeuron, &ONE, neuron->id - 1);
	);

	// Store the neuron of the agents 1 and 2 as we go through each neurons 
	Neuron* neuron_1;
	Neuron* neuron_2;

	// Used to compare links in neurons
	Link* link_1;
	Link* link_2;

	// Counter of matching links between two neurons
	int neuron_matching_links = 0;

	// node iterator
	clist* n_node;

	for (uint32_t i = 0; i < totalNeuron.count; i++)
	{
		// If neither agent has a neuron of id 'i+1' we got to the next neuron
		if (*(int*)vec_get(&totalNeuron, i) == 0)
			continue;

		neuron_1 = getNeuronInAgent(agent1, i + 1);
		neuron_2 = getNeuronInAgent(agent2, i + 1);

		if (neuron_1 == NULL)
		{
			disjoint += len(neuron_2->linkList);
		}
		else if (neuron_2 == NULL)
		{
			disjoint += len(neuron_1->linkList);
		}
		else
		{
			if (neuron_1->linkList != NULL)
			{
				neuron_matching_links = 0;

				// Cycle through the links in 'neuron_1'
				n_node = neuron_1->linkList;
				do
				{
					link_1 = (Link*)n_node->data;
					
					// If 'neuron_2' doesn't have the same link we increment disjoint and continue
					link_2 = getLinkInNeuron(neuron_2, pairToId(link_1->source->id, link_1->target->id));
					if (link_2 == NULL)
					{
						disjoint++;
					}
					else
					{						
						// Else we register the difference between links
						neuron_matching_links++;
						weight_diff += double_abs(link_1->weight - link_2->weight);
					}

					next(n_node);
				} while (n_node != neuron_1->linkList);
			}

			// For every neuron we count disjoint and matching links
			disjoint += len(neuron_2->linkList) - neuron_matching_links;
			matching += neuron_matching_links;	
		}		
	}

	weight_diff /= matching;
	total = matching + disjoint;

	free_vector(&totalNeuron);

	return disjoint * c1 / total + weight_diff * c2;
}

// TODO: use the same totalNeuron as in 'distance()'
// TODO: check if we can just cycle through links instead of neurons
// https://github.com/CodeReclaimers/neat-python/blob/c2b79c88667a1798bfe33c00dd8e251ef8be41fa/neat/genome.py#L234
Agent* crossOver(Agent* agent1, Agent* agent2)
{
    Agent* new_agent = request(&P_AGENT, sizeof(Agent));

    new_agent->fitness = 0;
    new_agent->neuronList = NULL;
    new_agent->linkList = NULL;
    new_agent->inputVector = new_vector(sizeof(Neuron*), INPUT_SIZE, 0);
    new_agent->outputVector = new_vector(sizeof(Neuron*), OUTPUT_SIZE, 0);

	vector totalNeuron_1 = new_vector(sizeof(int), NeuronCount, 0);
	vector totalNeuron_2 = new_vector(sizeof(int), NeuronCount, 0);
	memset(totalNeuron_1.start, 0, totalNeuron_1.type_size * totalNeuron_1.count);
	memset(totalNeuron_2.start, 0, totalNeuron_2.type_size * totalNeuron_2.count);

	// TODO: use '2' when both neurons exist
	ITER_V(agent1->neuronList, neuron_node, neuron, Neuron*,
		vec_set(&totalNeuron_1, &ONE, neuron->id - 1);
	);
	ITER_V(agent2->neuronList, neuron_node, neuron, Neuron*,
		vec_set(&totalNeuron_2, &ONE, neuron->id - 1);
	);


	bool exist_1;
	bool exist_2;
	for (uint32_t i = 0; i < NeuronCount; i++)
	{
		// WARNING: check the syntax im too drunk
		exist_1 = ((int*)totalNeuron_1.start)[i];
		exist_2 = ((int*)totalNeuron_2.start)[i];

		if (exist_1 == 1 && exist_2 == 0)
		{

		}
		else if (exist_1 == 0 && exist_2 == 1)
		{

		}
		else
		{
			
		}
	}

    return new_agent;
}

bool isNeuronInAgent(Agent* agent, uint32_t id)
{
	ITER_V(agent->neuronList, neuron_node, neuron, Neuron*,
		   if (neuron->id == id)
			   return true;
	);
	return false;
}

Neuron* getNeuronInAgent(Agent* agent, uint32_t id)
{
	ITER_V(agent->neuronList, neuron_node, neuron, Neuron*,
		   if (neuron->id == id)
			   return neuron;
	);
	return NULL;
}

bool addLinkInAgent(Agent* agent, uint32_t source, uint32_t target, double weight, bool enabled)
{
	Neuron* neuron_source = getNeuronInAgent(agent, source);
	Neuron* neuron_target = getNeuronInAgent(agent, target);

	if (neuron_source == NULL || neuron_target == NULL)
		return false;

	Link* new_link = new_Link(neuron_source, neuron_target, weight, enabled);
	insert(&agent->linkList, new_link);

	return true;
}

void free_agent(Agent** agent)
{
	// free links and linkList containers inside of neurons
	pclean(&(*agent)->linkList, &P_LINK);
	ITER((*agent)->neuronList, neuron_node,
		 clear(&((Neuron*)neuron_node->data)->linkList);
	);

	// free neurons
	pclean(&(*agent)->neuronList, &P_NEURON);

	// free IO vectors
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
		// if ((*(Neuron**)vec_get(&agent->inputVector, i))->linkList != NULL)
		if (((Neuron**)agent->inputVector.start)[i]->linkList != NULL)
			return false;
	}


	return true;
}