#include "neurolution/neuron.h"

#include "neurolution/neurolution.h"

#include "tools/utils.h"

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

	link->id = 0;
	link->source = source;
	link->target = target;
	link->weight = weight;
	link->enabled = enabled;

	cy_insert(&target->linkList, link);

	return link;
}

Link* new_LinkId(uint32_t source, uint32_t target, double weight, bool enabled)
{
	Link* link = request(&P_LINK, sizeof(Link));

	link->id = pairToId(source, target);
	link->source = NULL;
	link->target = NULL;
	link->weight = weight;
	link->enabled = enabled;

	return link;
}

Link* getLinkInNeuron(Neuron* neuron, int id)
{
	CY_ITER_DATA(neuron->linkList, link_node, link, Link*,
		   if (pairToId(link->source->id, link->target->id) == id)
			   return link;
	);
	return NULL;
}

Neuron* cloneNeuron(Neuron* neuron)
{
	Neuron* new_neuron = new_Neuron(
		neuron->id, 
		neuron->enabled, 
		neuron->activated, 
		neuron->type, 
		neuron->activationFunc, 
		neuron->value, 
		neuron->bias, 
		NULL);

	CY_ITER_DATA(neuron->linkList, link_node, link, Link*,
		cy_insert(&new_neuron->linkList,
			new_LinkId(
				link->source->id, 
				link->target->id, 
				link->weight, 
				link->enabled));
	);

	return new_neuron;
}