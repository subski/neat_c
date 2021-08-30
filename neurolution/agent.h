#ifndef AGENT_H
#define AGENT_H

#include "neurolution/env_settings.h"

typedef struct Neuron Neuron;
typedef struct clist clist;

typedef struct Agent Agent;

struct Agent
{
	double fitness;

	Neuron* inputNeurons[INPUT_SIZE];
	Neuron* outputNeurons[OUTPUT_SIZE];

	clist* neurons;
	clist* links;
};

Agent* createBasicAgent(void);

void free_agent(Agent** agent);

void print_agent(Agent* agent);

#endif