#include "mutations.h"

#include "neurolution/neurolution.h"
#include "neurolution/agent.h"
#include "neurolution/neuron.h"
#include "neurolution/env_settings.h"

#include "data_structures/clist.h"
#include "data_structures/pool.h"

#include "tools/pcg_basic.h"
#include "tools/utils.h"

#include "CPPN/activations.h"

// TODO: select neurons from a list of neurons.
// TODO: record metrics to check performance.
// TODO: mutate links only from enabled neurons.
// TODO: add toggle ON/OFF links.


bool mutate_link_add(Agent* agent)
{
	uint32_t neuron_count = cy_len(agent->neuronList);
	
	if (cy_len(agent->linkList) >= (neuron_count * (neuron_count-1)) - (neuron_count * agent->inputVector.count))
	{
		return false;
	}
	// Select two random neurons that are different and not input neurons.
	Neuron* neuron_source = cy_random_max(agent->neuronList, neuron_count);
	Neuron* neuron_target = cy_random_max(agent->neuronList, neuron_count-1);

	#pragma warning(disable : 4047 4022)
	if (neuron_source == neuron_target || neuron_target->type == INPUT_TYPE)
	{
		return false;
	}

	// Check if the two neurons are not connected to each other

	CY_ITER_DATA(neuron_target->linkList, node_link, link, Link*,
		if (link->source == neuron_source)
		{
			return false;
		}
	);

	CY_ITER_DATA(neuron_source->linkList, node_link, link, Link*,
		if (link->source == neuron_target)
		{
			return false;
		}
	);
	
	// Adding the link
	Link* new_link = new_Link(neuron_source, neuron_target, pcg32_doublerand() > 0.5 ? pcg32_doublerand() : pcg32_doublerand() * 0.01, true);
	cy_insert(&agent->linkList, new_link);

	return true;
}

/*
bool mutate_link_connect(Agent* agent)
{
	def check_pair(I, x):
    if x in I:
        ind = I.index(x)+1
        I[ind-1] = 0
        new_index = check_pair(I, ind)
        if not new_index:
            return ind
        return new_index
    return 0

	def add_random_pair(pair_list, pair_index, nb_item):
		k_max = int(nb_item*(nb_item-1))
		r_pair = randint(len(pair_list), k_max)
		index = check_pair(pair_index, r_pair)
		if index == 0:
			print('r:', r_pair - k_max/2)
			pair_list.append(idToPair(r_pair - k_max/2))
		else:
			print('i:', index - k_max/2, 'i:', index)
			pair_list.append(idToPair(index))
		pair_index.append(r_pair)
}
*/

void mutate_link_toggle(Agent* agent)
{
	Link* target_link = cy_random_data(agent->linkList);
	target_link->enabled ^= 1;
}

bool mutate_link_shift(Agent* agent, double shift)
{
	// Select random enabled link
	uint32_t link_count = cy_len(agent->linkList);
	Link* target_link = cy_random_max(agent->linkList, link_count);
	
	if (!target_link->enabled)
		return false;

	// shift the weight randomly
	target_link->weight += (shift * 2) * pcg32_doublerand() - shift;
	// target_link->weight = min(max(target_link->weight, 0), 1);

	return true;
}

void mutate_neuron_add(Agent* agent)
{
	// cy_insert(&agent->neuronList, new_BasicNeuron(++NeuronCount));
}

bool mutate_neuron_insert(Agent* agent, double (*activationFunc)(double))
{
	// Select random enabled link
	Link* target_link = cy_random_data(agent->linkList);
	if (!target_link->enabled)
		return false;


	int link_id = pairToId(target_link->source->id, target_link->target->id);

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
		CY_ITER_DATA(agent->neuronList, neuron_node, neuron, Neuron*,
			if (neuron->id == neuron_id)
				return false;
		);
	}

	// Split the original link and connect the new links to the new neurons

	target_link->enabled = false;

	Neuron* new_neuron = new_BasicNeuron(neuron_id, activationFunc);
	cy_insert(&agent->neuronList, new_neuron);

	Link* link_src = new_Link(target_link->source, new_neuron, 1.0, true);
	cy_insert(&agent->linkList, link_src);

	Link* link_target = new_Link(new_neuron, target_link->target, target_link->weight, true);
	cy_insert(&agent->linkList, link_target);

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
	if (pLINK_NEW > pcg32_doublerand())
	{
		for (int i=0; i<15 && !mutate_link_add(agent); i++ );
	}

	if (pLINK_SHIFT > pcg32_doublerand())
		mutate_link_shift(agent, 0.2);

	if (pLINK_TOGGLE > pcg32_doublerand())
		mutate_link_toggle(agent);
	
	if (pNODE_NEW > pcg32_doublerand())
		for (int i=0; i < 15 && !mutate_neuron_insert(agent, &ACT_FUNC); i++);
}