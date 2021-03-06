#ifndef NEURON_H
#define NEURON_H

#include "data_structures/clist.h"

/*
 * Classic NN neuron with a list of incoming connections ('links') and an activation function.
 */
typedef struct Neuron Neuron;


/*
 * Represent a connection between two nodes.
 */
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


struct Link
{
    int id;
    Neuron* source;
    Neuron* target;
	double weight;
	bool enabled;
};

// TODO: store links in an hashset
struct Neuron
{
    uint32_t id;
    uint32_t layer;


    bool enabled;
    bool activated;

    NeuronType type;

    double (*activationFunc)(double input);
    double value;
    double buffer;
    double bias;

    clist* linkList;
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

Neuron* new_BasicNeuron(uint32_t id, double (*activationFunc)(double));

Link* new_Link(Neuron* source, Neuron* target, double weight, bool enabled);

Link* new_LinkId(uint32_t source, uint32_t target, double weight, bool enabled);

Link* getLinkInNeuron(Neuron* neuron, int id);

Link* getLinkInList(clist* neuronlist, int id);

Neuron* clone_neuron(Neuron* neuron, bool cloneLinks);

void neuron_activate(Neuron* neuron);

void neuron_update(Neuron* neuron);

void print_link_id_matrix(int size);

#endif