#include "tasks/XorEvaluator.h"

#include <stdio.h>

#include "neurolution/agent.h"
#include "neurolution/neuron.h"

#include "data_structures/vector.h"

#include "tools/utils.h"

const double DATASET[4][4] = {
    { 1, 0, 0, 0 },
    { 1, 0, 1, 1 },
    { 1, 1, 0, 1 },
    { 1, 1, 1, 0 }
};

double XorEvaluator(Agent* agent)
{
    double fitness = 0.0;
    bool success = true;

    // Test case 0, 0.
    vector* outputs = agent_eval(agent, DATASET[0], 10);
    double output = min(max(VEC((*outputs), Neuron*, 0)->value, 0.0), 1.0);
    success &= output <= 0.5;
    fitness += 1.0 - (output * output);
    // printf("fitness: %lf\n", fitness);

    // Test case 0, 1.
    outputs = agent_eval(agent, DATASET[1], 10);
    output = min(max(VEC((*outputs), Neuron*, 0)->value, 0.0), 1.0);
    success &= output > 0.5;
    fitness += 1.0 - ((1.0 - output) * (1.0 - output));

    // Test case 1, 0.
    outputs = agent_eval(agent, DATASET[2], 10);
    output = min(max(VEC((*outputs), Neuron*, 0)->value, 0.0), 1.0);
    success &= output > 0.5;
    fitness += 1.0 - ((1.0 - output) * (1.0 - output));

    
    // Test case 1, 1.
    outputs = agent_eval(agent, DATASET[3], 10);
    output = min(max(VEC((*outputs), Neuron*, 0)->value, 0.0), 1.0);
    success &= output <= 0.5;
    fitness += 1.0 - (output * output);
    
    if(success) {
        fitness += 10.0;
    }

    return fitness;
}