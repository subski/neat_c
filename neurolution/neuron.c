#include "neurolution/neuron.h"

Neuron* createNeuron_f(
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

	return neuron;
}

Neuron* createNeuron(Neuron* neuron, unsigned short id)
{
	return createNeuron_f(neuron, id, true, false, HIDDEN_TYPE, NULL, 0.0, 0.0, NULL);
}