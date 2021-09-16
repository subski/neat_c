#include "mutations.h"

#include "neurolution/neurolution.h"
#include "neurolution/agent.h"
#include "neurolution/neuron.h"

#include "data_structures/clist.h"
#include "data_structures/pool.h"

#include "tools/pcg_basic.h"
#include "tools/utils.h"

// TODO: select neurons from a list of neurons.
// TODO: record metrics to check performance.
// TODO: mutate links only from enabled neurons.
// TODO: add toggle ON/OFF links.


bool mutate_link_add(Agent* agent)
{
	// Select two random neurons that are different and not input neurons.
	uint32_t neuron_count = len(agent->neuronList);
	Neuron* neuron_source = random_max(agent->neuronList, neuron_count);
	Neuron* neuron_target = random_max(agent->neuronList, neuron_count);

	if (neuron_source == neuron_target || isPtrInArray(neuron_target, agent->inputVector.start, INPUT_SIZE))
	{
		return false;
	}

	// Check if the two neurons are not connected to each other

	ITER_V(neuron_target->linkList, node_link, link, Link*,
		if (link->source == neuron_source)
			return false;
	);

	ITER_V(neuron_source->linkList, node_link, link, Link*,
		if (link->source == neuron_target)
			return false;
	);
	
	// Adding the link
	Link* new_link = new_Link(neuron_source, neuron_target, 1.0, true);
	insert(&agent->linkList, new_link);

	return true;
}

void mutate_link_toggle(Agent* agent)
{
	Link* target_link = random_data(agent->linkList);
	target_link->enabled ^= 1;
}

bool mutate_link_shift(Agent* agent, double shift)
{
	// Select random enabled link
	uint32_t link_count = len(agent->linkList);
	Link* target_link = random_max(agent->linkList, link_count);
	
	if (!target_link->enabled)
		return false;

	// shift the weight randomly
	target_link->weight += (shift * 2) * pcg32_doublerand() - shift;

	return true;
}

void mutate_neuron_add(Agent* agent)
{
	insert(&agent->neuronList, new_BasicNeuron(++NeuronCount));
}

bool mutate_neuron_insert(Agent* agent)
{
	// Select random enabled link
	Link* target_link = random_data(agent->linkList);
	if (!target_link->enabled)
		return false;


	uint32_t link_id = pairToId(target_link->source->id, target_link->target->id);

	NeuronHistory_s* neuron_node = NeuronHistory.next;
	uint32_t neuron_id = 0;

	// Cycle through the history of neurons
	// to check for existing neuron id
	while (neuron_node)
	{
		if (neuron_node->linkId == link_id)
		{
			neuron_id = neuron_node->neuronId;
			break;
		}
		neuron_node = neuron_node->next;
	}

	// If no neuron id was found, we add a new one to the history
	// else we check if the id we found is not already part of the agent
	if (!neuron_id)
	{
		neuron_id = ++NeuronCount;

		NeuronHistory_s* new_node = malloc(sizeof(NeuronHistory_s));
		new_node->linkId = link_id;
		new_node->neuronId = neuron_id;
		new_node->next = NeuronHistory.next;
		NeuronHistory.next = new_node;
	}
	else
	{
		ITER_V(agent->neuronList, neuron_node, neuron, Neuron*,
			if (neuron->id == neuron_id)
				return false;
		);
	}

	// Split the original link and connect the new links to the new neurons

	target_link->enabled = false;

	Neuron* new_neuron = new_BasicNeuron(neuron_id);
	insert(&agent->neuronList, new_neuron);

	Link* link_src = new_Link(target_link->source, new_neuron, target_link->weight, true);
	insert(&agent->linkList, link_src);

	Link* link_target = new_Link(new_neuron, target_link->target, 0.0, true);
	insert(&agent->linkList, link_target);

	return true;
}

void mutate_neuron_toggle(Agent* agent)
{
}

void mutate_neuron_bias(Agent* agent)
{

}

void mutate_agent(Agent* agent)
{
	if (0.3 > pcg32_doublerand())
		mutate_link_add(agent);
	
	if (0.85 > pcg32_doublerand())
		mutate_link_shift(agent, 0.2);
	
	if (0.025 > pcg32_doublerand())
		mutate_link_toggle(agent);
	
	if (0.01 > pcg32_doublerand())
		mutate_neuron_insert(agent);
}