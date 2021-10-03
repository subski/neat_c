#include "tasks/XorEvaluator.h"

#include <stdio.h>

#include "neurolution/agent.h"

#include "data_structures/vector.h"

#include "tools/utils.h"

void XorEvaluator(Agent* agent)
{
    ColorRed();
    printf("Evaluating\n");
    ColorReset();

    double dataset[4][4] = {
        { 0, 0, 1, 0 },
        { 0, 1, 1, 1 },
        { 1, 0, 1, 1 },
        { 1, 1, 1, 0 }
    };

    agent_eval(agent, dataset[0]);
}