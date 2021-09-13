#ifndef AGENT_H
#define AGENT_H

#include "data_structures/vector.h"

#include "neurolution/env_settings.h"

typedef struct Neuron Neuron;
typedef struct clist clist;

typedef struct Agent Agent;

// TODO: replace neurons array by vectors
struct Agent
{
	double fitness;

	vector inputVector;
	vector outputVector;

	clist* neuronList;
	clist* linkList;
};

Agent* new_BasicAgent(uint32_t inputCount, uint32_t outputCount);

void free_agent(Agent** agent);

bool check_agent(Agent* agent);

void print_agent(Agent* agent);

#endif