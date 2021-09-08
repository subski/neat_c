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
// TODO: mutate links only from enabled neurons

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

	Link* new_link    = request(&P_LINK, sizeof(Link));
	new_link->source  = neuron_source;
	new_link->target  = neuron_target;
	new_link->weight  = 1.0;
	new_link->enabled = true;

	insert(&neuron_target->links, new_link);
	insert(&agent->links, new_link);

	return true;
}

void mutate_link_toggle(Agent* agent)
{
	Link* target_link = random(agent->links);
	target_link->enabled ^= 1;
}

void mutate_link_shift(Agent* agent, double shift)
{
	uint32_t link_count = len(agent->links);
	Link* target_link = random_max(agent->links, link_count);
	while (!target_link->enabled)
		target_link = random_max(agent->links, link_count);

	target_link->weight += (shift * 2) * pcg32_doublerand() - shift;
}

void mutate_neuron_add(Agent* agent)
{
	NeuronCount++;
	insert(&agent->neurons, createNeuron(request(&P_NEURON, sizeof(Neuron)), NeuronCount));
}

bool mutate_neuron_insert(Agent* agent)
{

	return true;
}

void mutate_neuron_toggle(Agent* agent)
{
}

void mutate_neuron_bias(Agent* agent)
{

}
