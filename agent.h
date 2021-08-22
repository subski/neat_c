#ifndef AGENT_H
#define AGENT_H

#include "clist.h"
#include "neuron.h"
#include "env_settings.h"

typedef struct Agent Agent;

struct Agent
{
	double fitness;
	Neuron* inputNeurons[INPUT_SIZE];
	Neuron* outputNeurons[OUTPUT_SIZE];
	clist* neurons;
	clist* links;
};

#endif