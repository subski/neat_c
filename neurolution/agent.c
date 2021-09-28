#include "neurolution/agent.h"

#include <assert.h>

#include "data_structures/clist.h"
#include "data_structures/pool.h"
#include "data_structures/vector.h"

#include "neurolution/neuron.h"
#include "neurolution/neurolution.h"

#include "tools/utils.h"
#include "tools/pcg_basic.h"

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
				VEC(new_agent->inputVector, Neuron*, j),
				VEC(new_agent->outputVector, Neuron*, i),
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
	vector totalNeuron = new_vector(sizeof(byte_t), NeuronCount, 0);
	memset(totalNeuron.start, 0, totalNeuron.type_size * totalNeuron.count);

	ITER_V(agent1->neuronList, neuron_node, neuron, Neuron*,
		VEC(totalNeuron, byte_t*, neuron->id-1) = 1;
	);
	ITER_V(agent2->neuronList, neuron_node, neuron, Neuron*,
		VEC(totalNeuron, byte_t*, neuron->id-1) += 2;
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
		switch (VEC(totalNeuron, byte_t*, i))
		{
            case 0: // No neurons in common
                continue;
            break;
            case 1: // Neuron in agent1 only
			    disjoint += len(getNeuronInAgent(agent1, i+1)->linkList);
            break;
            case 2: // Neuron in agent2 only
		        disjoint += len(getNeuronInAgent(agent2, i+1)->linkList);
            break;
            case 3: // Neuron in both agents
                neuron_1 = getNeuronInAgent(agent1, i + 1);
		        neuron_2 = getNeuronInAgent(agent2, i + 1);
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
            break;
            default:
				assert("agent.c > distance(): totalNeuron id incorrect.");
                continue;
            break;
        }
	}

	weight_diff /= matching;
	total = matching + disjoint;

	free_vector(&totalNeuron);

	return disjoint * c1 / total + weight_diff * c2;
}

Agent* crossOver(Agent* agent1, Agent* agent2)
{
	// Swap agents to always have 'agent1' as the fittest agent.
	if (agent1->fitness < agent2->fitness)
	{
		Agent* tmp_agent = agent1;
		agent1 = agent2;
		agent2 = tmp_agent;
	}

	// "Child" agent.
    Agent* new_agent = request(&P_AGENT, sizeof(Agent));

    new_agent->fitness = 0;
    new_agent->neuronList = NULL;
    new_agent->linkList = NULL;
    new_agent->inputVector = new_vector(sizeof(Neuron*), INPUT_SIZE, 0);
    new_agent->outputVector = new_vector(sizeof(Neuron*), OUTPUT_SIZE, 0);

	// 'totalNeuron' is a summary of which neuron is present in which agents.
	// '0': no neurons of id [index of vector] in either agents
	// '1': neuron is present in 'agent1' only
	// '2': neuron is present in 'agent2' only
	// '3': neuron is present in both agents
	vector totalNeuron = new_vector(sizeof(byte_t), NeuronCount, 0);
	memset(totalNeuron.start, 0, totalNeuron.type_size * totalNeuron.count);

	ITER_V(agent1->neuronList, neuron_node, neuron, Neuron*,
		VEC(totalNeuron, byte_t, neuron->id-1) = 1;
	);
	ITER_V(agent2->neuronList, neuron_node, neuron, Neuron*,
		VEC(totalNeuron, byte_t, neuron->id-1) += 2;
	);


	// Iterate through the 'totalNeuron' sequence and form new neurons from agents
	Neuron *neuron_1, *neuron_2, *new_neuron;
	Link* link_2;
	for (uint32_t i = 0; i < totalNeuron.count; i++)
	{
		switch (VEC(totalNeuron, byte_t, i))
		{
			case 0: // No neuron
				continue;
			break;
			case 1: // Neuron in agent1 only
				// We copy the neuron to the childe 1 to 1
				new_neuron = cloneNeuron(getNeuronInAgent(agent1, i+1));
			break;
			case 2: // Neuron in agent2 only
				continue;
			break;
			case 3: // Neuron in both agents
				neuron_1 = getNeuronInAgent(agent1, i+1);
				neuron_2 = getNeuronInAgent(agent2, i+1);
				new_neuron = new_Neuron(
					neuron_1->id, 
					neuron_1->enabled, 
					neuron_1->activated, 
					neuron_1->type, 
					neuron_1->activationFunc, 
					neuron_1->value, 
					(neuron_1->bias + neuron_2->bias) / 2, // TODO: check bias crossover 
					NULL);

				// We iterate through all the links of both neurons and we chose at random links from the neuron1 links.
				ITER_V(neuron_1->linkList, link_node, link_1, Link*,
					link_2 = getLinkInNeuron(neuron_2, pairToId(link_1->source->id, link_1->target->id));
					if (link_2 == NULL || pcg32_doublerand() < 0.5)
					{
                        insert(&new_neuron->linkList,
                            new_LinkId(
                                link_1->source->id, 
                                link_1->target->id, 
                                link_1->weight, 
                                link_1->enabled));
					}
					else
					{
                        insert(&new_neuron->linkList,
                            new_LinkId(
                                link_2->source->id, 
                                link_2->target->id, 
                                link_2->weight, 
                                link_2->enabled));
					}
				);
			break;
			default:
                continue; // TODO: raise error
			break;
		}
		// Add the neuron to the child agent
        insert(&new_agent->neuronList, new_neuron);
	}

    free_vector(&totalNeuron);

	// Actually initialize the links in the child. There is probably a better way.
    uint32_t p1, p2;
    ITER_V(new_agent->neuronList, neuron_node, neuron, Neuron*,
        ITER_V(neuron->linkList, link_node, link, Link*,
            insert(&new_agent->linkList, link);
            idToPair(link->id, &p1, &p2);
            link->source = getNeuronInAgent(new_agent, p1);
            link->target = neuron;
        );
    );

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
		if (VEC(agent->inputVector, Neuron*, i)->linkList != NULL)
			return false;
	}

	return true;
}