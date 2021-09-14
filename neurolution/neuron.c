#include "neurolution/neuron.h"

#include "neurolution/neurolution.h"

#include "data_structures/clist.h"
#include "data_structures/pool.h"

Neuron* new_Neuron(
	uint32_t id,
	bool enabled,
	bool activated,
	NeuronType type,
	double (*activationFunc)(double),
	double value,
	double bias, 
	clist* links)
{
	Neuron* neuron = request(&P_NEURON, sizeof(Neuron));

	neuron->id             = id;
	neuron->enabled        = enabled;
	neuron->activated      = activated;
	neuron->type           = type;
	neuron->activationFunc = activationFunc;
	neuron->value          = value;
	neuron->bias           = bias;
	neuron->linkList       = links;

	return neuron;
}

Neuron* new_BasicNeuron(uint32_t id)
{
	return new_Neuron(id, true, false, HIDDEN_TYPE, NULL, 0.0, 0.0, NULL);
}

Link* new_Link(Neuron* source, Neuron* target, double weight, bool enabled)
{
	Link* link = request(&P_LINK, sizeof(Link));

	link->source = source;
	link->target = target;
	link->weight = weight;
	link->enabled = enabled;

	insert(&target->linkList, link);

	return link;
}

Link* getLinkInNeuron(Neuron* neuron, uint32_t id)
{
	ITER_V(neuron->linkList, link_node, link, Link*,
		   if (pairToId(link->source->id, link->target->id) == id)
			   return link;
	);
	return NULL;
}