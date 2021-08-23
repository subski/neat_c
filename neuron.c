#include "neuron.h"

void createNeuron(
	Neuron* neuron,
	unsigned short id,
	bool enabled,
	bool activated,
	NeuronType type,
	double (*activationFunc)(double),
	double value,
	double bias,
	clist* links)
{
	neuron->id = id;
	neuron->enabled = enabled;
	neuron->activated = activated;
	neuron->type = type;
	neuron->activationFunc = activationFunc;
	neuron->value = value;
	neuron->bias = bias;
	neuron->links = links;
}