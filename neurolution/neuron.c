#include "neurolution/neuron.h"

#include "neurolution/neurolution.h"

#include "tools/utils.h"

#include "data_structures/clist.h"
#include "data_structures/pool.h"

#include "CPPN/activations.h"

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
	return new_Neuron(id, true, false, HIDDEN_TYPE, &fast_tanh, 0.0, 0.0, NULL);
}

Link* new_Link(Neuron* source, Neuron* target, double weight, bool enabled)
{
	Link* link = request(&P_LINK, sizeof(Link));

	link->id = pairToId(source->id, target->id);
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
	return getLinkInList(neuron->linkList, id);
}

Link* getLinkInList(clist* neuronlist, int id)
{
	CY_ITER_DATA(neuronlist, link_node, link, Link*,
		   if (link->id == id)
			   return link;
	);
	return NULL;
}



Neuron* clone_neuron(Neuron* neuron)
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

void neuron_activate(Neuron* neuron)
{
	neuron->buffer = 0;
	CY_ITER_DATA(neuron->linkList, link_node, link, Link*,
		if (link->enabled)
		{
			printf("%lf * %lf\n", link->source->value, link->weight);
			neuron->buffer += link->source->value * link->weight;
		}
	);
	neuron->buffer = neuron->activationFunc(neuron->buffer);
}

void neuron_update(Neuron* neuron)
{
	neuron->value = neuron->buffer;
}


void print_link_id_matrix(int size)
{
	printf(" . ");
	for (int i = 1; i < size; i++)
		printf("  %d  ", i);
	NEWLINE();
	for (int i = 1; i < size; i++)
	{
		printf(" %d ", i);
		for (int j = 1; j < size; j++)
		{
			printf(" %03d ", pairToId(j, i));
		}
		NEWLINE();	
	}
}