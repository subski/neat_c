#include "tasks/XorEvaluator.h"

#include <stdio.h>

#include "neurolution/agent.h"
#include "neurolution/neuron.h"

#include "data_structures/vector.h"

#include "tools/utils.h"

#include "CPPN/activations.h"

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
    vector* outputVector;
    double output;

    for (double i = 0; i < 1.0; i += 0.001)
    {
        outputVector = agent_eval(agent, &i, 10);
        output = VEC((*outputVector), Neuron*, 0)->value;
        fitness += 1.0 - double_abs(i - output*output);
    }


    // for (int i = 0; i < 4; i++)
    // {
    //     outputVector = agent_eval(agent, DATASET[i], 50);
    //     output = VEC((*outputVector), Neuron*, 0)->value;
    //     output = min(max(output, 0.0), 1.0);

    //     if (DATASET[i][3] == 1)
    //     {
    //         success &= output > 0.5;
    //         fitness += 1.0 - ((1.0 - output) * (1.0 - output));
    //     }
    //     else
    //     {
    //         success &= output <= 0.5;
    //         fitness += 1.0 - (output * output);
    //     }
    // }
    
    // if(success) {
    //     fitness += 10.0;
    // }

    return fitness;
}