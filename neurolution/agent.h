#ifndef AGENT_H
#define AGENT_H

#include "data_structures/vector.h"

#include "neurolution/env_settings.h"

typedef struct Neuron Neuron;
typedef struct clist clist;

typedef struct Agent Agent;

struct Agent
{
    int specie;

    bool survive;

    double fitness;

    vector inputVector;
    vector outputVector;

    clist* neuronList;
    clist* linkList;
};

/*
    Create an empty Agent with empty input/output vectors.
*/
Agent* new_Agent(uint32_t inputSize, uint32_t outputSize);

/*
    Create an Agent with inputs connected to the outputs.
*/
Agent* new_BasicAgent(uint32_t inputSize, uint32_t outputSize, double (*activationFunc)(double));

// TODO agent eval
vector* agent_eval(Agent* agent, const double inputs[], int max_step);

/*
    Mesure the euclidian distance between two agents.
*/
double agent_euclidean_distance(Agent* agent1, Agent* agent2);

/*
    Create a new Agent from two existing agents.
*/
Agent* crossOver(Agent* agent1, Agent* agent2);

/*
    Search for a neuron id in the 'neuronList' of an agent.
    Return 1 if exist, otherwise 0.
*/
bool isNeuronInAgent(Agent* agent, uint32_t id);

/*
    Search for a neuron id in the 'neuronList' of an agent.
    Returns the neuron found or NULL.
*/
Neuron* getNeuronInAgent(Agent* agent, uint32_t id);

/*
    Add a Link to an agent.
*/
bool addLinkInAgent(Agent* agent, uint32_t source, uint32_t target, double weight, bool enabled);

/*
    Free memory used by links and neurons of an Agent.
*/
void free_agent(Agent** agent);

/*
    Verify the integrity on an Agent.
*/
int check_agent(Agent* agent);

/*
    Create a copy of a given agent.
*/
Agent* agent_clone(Agent* agent);

/*
    Returns the sum of the weights of all links in an Agent.
*/
double agent_weight(Agent* agent);

#endif