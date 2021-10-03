#include "tasks/XorEvaluator.h"

#include <stdio.h>

#include "neurolution/agent.h"
#include "neurolution/neuron.h"

#include "data_structures/vector.h"

#include "tools/utils.h"

const double DATASET[4][4] = {
    { 0, 0, 1, 0 },
    { 0, 1, 1, 1 },
    { 1, 0, 1, 1 },
    { 1, 1, 1, 0 }
};

double XorEvaluator(Agent* agent)
{
    vector* output;
    double fitness = 0;
    for (int i = 0; i < 4; i++)
    {
        output = agent_eval(agent, DATASET[i], 100);
        fitness += VEC((*output), Neuron*, 0)->value;
    }

    return double_abs(fitness);
}