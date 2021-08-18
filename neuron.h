#ifndef NEURON_H
#define NEURON_H

#include <stdbool.h>

typedef struct Neuron Neuron;
typedef enum NeuronType NeuronType;
typedef struct Link Link;

/*
 * Helper to differenciate different neurons type.
 */
enum NeuronType
{
    INPUT,
    OUTPUT,
    HIDDEN
};

/*
 * Represent a connection between a two nodes.
 */ 
struct Link
{
    struct Link* next;
    Neuron* source;
    double weight;
    bool activated;
};

/*
 * Store a neuron with references to links with other neurons.
 */
struct Neuron 
{
    unsigned id;
    bool enabled;
    double (*activationFunc)(double input);
    double bias;
    NeuronType type;
    Link* links;
    double value;
    bool activated;

};

#endif