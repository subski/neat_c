#ifndef AGENT_H
#define AGENT_H

#include "data_structures/vector.h"

#include "neurolution/env_settings.h"

typedef struct Neuron Neuron;
typedef struct clist clist;

typedef struct Agent Agent;

// TODO: add documentation for agent.h
struct Agent
{
    double fitness;

    vector inputVector;
    vector outputVector;

    clist* neuronList;
    clist* linkList;
};

Agent* new_Agent(uint32_t inputSize, uint32_t outputSize);

Agent* new_BasicAgent(uint32_t inputSize, uint32_t outputSize);

double distance(Agent* agent1, Agent* agent2, double c1, double c2);

Agent* crossOver(Agent* agent1, Agent* agent2);

bool isNeuronInAgent(Agent* agent, uint32_t id);

Neuron* getNeuronInAgent(Agent* agent, uint32_t id);

bool addLinkInAgent(Agent* agent, uint32_t source, uint32_t target, double weight, bool enabled);

void free_agent(Agent** agent);

bool check_agent(Agent* agent);

#endif