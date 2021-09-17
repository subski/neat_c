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
				clist* n_node = neuron_1->linkList;
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

Agent* crossOver(Agent* agent1, Agent* agent2)
{
    Agent* new_agent = request(&P_AGENT, sizeof(Agent));
	int a = 0;
    new_agent->fitness = 0;
    new_agent->neuronList = NULL;
    new_agent->linkList = NULL;
    new_agent->inputVector = new_vector(sizeof(Neuron*), INPUT_SIZE, 0);
    new_agent->outputVector = new_vector(sizeof(Neuron*), OUTPUT_SIZE, 0);

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
		// if ((*(Neuron**)vec_get(&agent->inputVector, i))->linkList != NULL)
		if (((Neuron**)agent->inputVector.start)[i]->linkList != NULL)
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

	printf("Neurons: %d | Links: %d | Fitness: %lf\n\n", len(agent->neuronList), len(agent->linkList), agent->fitness);
}

// TODO: load special activation function from file
// TODO: log enabled/disabled neurons and bias
// TODO: maybe different format for neurons like: 'neuron_id': 'incoming connections'
// TODO: log enabled/disabled connections information
// TODO: retrive inputs/outputs links and save them in inputVector/outputVector
bool save_agent(char filename[], Agent* agent)
{
	FILE* target_file = fopen(filename, "w");
	if (target_file == NULL)
	{
		fprintf(stderr, "Cannot write agent data in file '%s'.", filename);
		return false;
	}

	fprintf(target_file, "# Input and output node counts.\n");
	fprintf(target_file, "%d\t%d\n\n", INPUT_SIZE, OUTPUT_SIZE);

	fprintf(target_file, "# Connections (source target weight).\n");
	ITER_V(agent->linkList, link_node, link, Link*,
		fprintf(target_file, "%d %d %lf\n", link->source->id, link->target->id, link->weight);
	);

	fprintf(target_file, ";\n\n");

	fprintf(target_file, "# Activation functions (functionId functionCode).\n");
	fprintf(target_file, "0 None\n\n");

	fputc(EOF, target_file);

	fclose(target_file);
	return true;
}

Agent* load_agent(char filename[])
{
	Agent* new_agent;
	char line[255];

	uint32_t inputSize = 0, outputSize = 0;
	uint32_t id_source, id_target;
	Neuron* neuron_source;
	Neuron* neuron_target;
	double weight;
	uint32_t activationFunctionId;
	char activationFunctionName[255];

	bool r_ioSize = false, r_connections = false, r_activationFunction = false;

	FILE* target_file = fopen(filename, "r");

	while (fgets(line, sizeof(line), target_file) != NULL)
	{
		if (line[0] == '#' || line[0] == '\n')
			continue;
		
		if (!r_ioSize)
		{
			sscanf(line, "%d\t%d\n", &inputSize, &outputSize);
			new_agent = new_Agent(inputSize, outputSize);

			printf("D> inputSize: %d, outputSize: %d\n", inputSize, outputSize);

			r_ioSize = true;
			continue;
		}

		if (!r_connections)
		{
			if (line[0] == ';')
			{
				r_connections = true;
				continue;
			}

			sscanf(line, "%d\t%d\t%lf\n", &id_source, &id_target, &weight);
			
			printf("D> id_source: %d, id_target: %d, weight: %lf\n", id_source, id_target, weight);

			if (!isNeuronInAgent(new_agent, id_source))
			{
				neuron_source = new_BasicNeuron(id_source);
				insert(&new_agent->neuronList, neuron_source);
			}
			else
			{
				neuron_source = getNeuronInAgent(new_agent, id_source);
			}

			if (!isNeuronInAgent(new_agent, id_target))
			{
				neuron_target = new_BasicNeuron(id_target);
				insert(&new_agent->neuronList, neuron_target);
			}
			else
			{
				neuron_target = getNeuronInAgent(new_agent, id_target);
			}

			insert(&new_agent->linkList, new_Link(neuron_source, neuron_target, weight, true));
			
			// r_connections = true;
			continue;
		}

		if (!r_activationFunction)
		{
			r_activationFunction = true;
			continue;
		}
	}

	fclose(target_file);
	return new_agent;
}