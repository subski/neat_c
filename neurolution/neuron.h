#ifndef NEURON_H
#define NEURON_H

typedef struct clist clist;

typedef struct Neuron Neuron;
typedef struct Link Link;

/*
 * Helper to differenciate different neurons type.
 */
typedef enum NeuronType
{
	INPUT_TYPE,
	OUTPUT_TYPE,
	HIDDEN_TYPE
} NeuronType;

/*
 * Represent a connection between two nodes.
 */
struct Link
{
    Neuron* source;
    Neuron* target;
	double weight;
	bool enabled;
};

/*
 * Store a neuron with references to links with other neurons.
 */
struct Neuron
{
    uint32_t id;

    bool enabled;
    bool activated;

    NeuronType type;

    double (*activationFunc)(double input);
    double value;
    double bias;

    clist* links;
};

Neuron* new_Neuron(
    uint32_t id,

    bool enabled,
    bool activated,

    NeuronType type,

    double (*activationFunc)(double),
    double value,
    double bias,

    clist* links);

Neuron* new_BasicNeuron(uint32_t id);

Link* new_Link(Neuron* source, Neuron* target, double weight, bool enabled);

#endif