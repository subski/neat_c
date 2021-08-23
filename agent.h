#pragma once
#ifndef AGENT_H
#define AGENT_H

#include "env_settings.h"

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

#endif