#ifndef NEURON_H
#define NEURON_H

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
 * Represent a connection between two nodes.
 */ 
struct Link
{
    Link* next;
    Neuron* source;
    double weight;
    bool activated;
};

/*
 * Store a neuron with references to links with other neurons.
 */
struct Neuron 
{
    uint16_t id;
    bool enabled;
    bool activated;
    NeuronType type;
    double (*activationFunc)(double input);
    double value;
    double bias;
    Link* links;

};

#endif