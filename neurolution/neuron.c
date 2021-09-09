#include "neurolution/neuron.h"

Neuron* createNeuron_f(
	Neuron* neuron,
	uint32_t id,
	bool enabled,
	bool activated,
	NeuronType type,
	double (*activationFunc)(double),
	double value,
	double bias,
	clist* links)
{
	neuron->id             = id;
	neuron->enabled        = enabled;
	neuron->activated      = activated;
	neuron->type           = type;
	neuron->activationFunc = activationFunc;
	neuron->value          = value;
	neuron->bias           = bias;
	neuron->links          = links;

	return neuron;
}

Neuron* createNeuron(Neuron* neuron, uint32_t id)
{
	return createNeuron_f(neuron, id, true, false, HIDDEN_TYPE, NULL, 0.0, 0.0, NULL);
}

Link* createLink(Link* link, Neuron* source, Neuron* target, double weight, bool enabled)
{
	link->source = source;
	link->target = target;
	link->weight = weight;
	link->enabled = enabled;

	return link;
}