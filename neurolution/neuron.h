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
	double weight;
	bool enabled;
};

/*
 * Store a neuron with references to links with other neurons.
 */
struct Neuron
{
    unsigned   short id;

    bool enabled;
    bool activated;

    NeuronType type;

    double (*activationFunc)(double input);
    double value;
    double bias;

    clist* links;
};

Neuron* createNeuron_f(
    Neuron* neuron,
    unsigned short id,

    bool enabled,
    bool activated,

    NeuronType type,

    double (*activationFunc)(double),
    double value,
    double bias,

    clist* links);

Neuron* createNeuron(Neuron* neuron, unsigned short id);

#endif