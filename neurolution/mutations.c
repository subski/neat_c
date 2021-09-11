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
	uint32_t neuron_count = len(agent->neurons);
	Neuron* neuron_source = random_max(agent->neurons, neuron_count);
	Neuron* neuron_target = random_max(agent->neurons, neuron_count);

	if (neuron_source == neuron_target || ptrInArray(neuron_target, agent->inputNeurons, INPUT_SIZE))
	{
		return false;
	}

	ITER_V(neuron_target->links, node_link, link, Link*,
		if (link->source == neuron_source)
			return false;
	);

	ITER_V(neuron_source->links, node_link, link, Link*,
		if (link->source == neuron_target)
			return false;
	);

	Link* new_link = new_Link(neuron_source, neuron_target, 1.0, true);
	insert(&agent->links, new_link);

	return true;
}

void mutate_link_toggle(Agent* agent)
{
	Link* target_link = random(agent->links);
	target_link->enabled ^= 1;
}

bool mutate_link_shift(Agent* agent, double shift)
{
	uint32_t link_count = len(agent->links);
	Link* target_link = random_max(agent->links, link_count);
	
	if (!target_link->enabled)
		return false;
	
	target_link = random_max(agent->links, link_count);
	target_link->weight += (shift * 2) * pcg32_doublerand() - shift;

	return true;
}

void mutate_neuron_add(Agent* agent)
{
	insert(&agent->neurons, new_BasicNeuron(++NeuronCount));
}

bool mutate_neuron_insert(Agent* agent)
{
	Link* target_link = random(agent->links);
	if (!target_link->enabled)
		return false;

	uint32_t neuron_id = pairToId(target_link->source->id, target_link->target->id);

	ITER_V(agent->neurons, neuron_node, neuron, Neuron*,
		if (neuron->id == neuron_id)
			return false;
	);

	target_link->enabled = false;

	Neuron* new_neuron = new_BasicNeuron(++NeuronCount);
	insert(&agent->neurons, new_neuron);

	Link* link_src = new_Link(target_link->source, new_neuron, target_link->weight, true);
	insert(&agent->links, link_src);

	Link* link_target = new_Link(new_neuron, target_link->target, 0.0, true);
	insert(&agent->links, link_target);

	return true;
}

void mutate_neuron_toggle(Agent* agent)
{
}

void mutate_neuron_bias(Agent* agent)
{

}
