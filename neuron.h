#pragma once
#ifndef NEURON_H
#define NEURON_H

#include <stdbool.h>
#include <stdint.h>

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
	clist* links;
};

#endif