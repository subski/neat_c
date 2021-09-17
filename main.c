#include "tools/utils.h"
#include "tools/pcg_basic.h"

#include "data_structures/clist.h"
#include "data_structures/pool.h"
#include "data_structures/vector.h"

#include "neurolution/env_settings.h"
#include "neurolution/neuron.h"
#include "neurolution/agent.h"
#include "neurolution/neurolution.h"
#include "neurolution/mutations.h"


#ifdef _WIN32
#include <sys\timeb.h> 
struct timeb progStart, progEnd;
#endif

void onExit(void)
{
    free_neurolution();

#if USE_DEBUG_MALLOC
    printLeaks();
#endif

#ifdef _WIN32
    ftime(&progEnd);
    
    printf("\n\n---------------------------------------------------------------------------------------------------------------------\n");
    printf("Program ended in %.3f seconds.\n", (1000 * (progEnd.time - progStart.time) + (progEnd.millitm - progStart.millitm)) / 1000.f);
    system("pause");
#endif
}

void onStart(void)
{
    atexit(&onExit);
    pcg32_srandom((unsigned long long)time(NULL), (unsigned long long) & printf);

#ifdef _WIN32
    ftime(&progStart);
#endif
}

bool test()
{
    // NEXT: plot agent
    // NEXT: crossover function
    Agent* agent1;
    
    //agent1 = load_agent("test.genome");

    print_agent(agent1)

    // throw segmentation fault because we don't initialize inputVector when loading the agent.
    // if (!check_agent(agent1))
    // {
    //     printf("AGENT CORRUPTED!\n");
    // }

    free_agent(&agent1);
    return false;
}

int main(void)
{
    onStart();

    if (!test()) return 0;
    
    evolve();

    Agent* agent = * (Agent**) Population.start;

    /*mutate_neuron_insert(agent);
    mutate_neuron_insert(agent);
    mutate_neuron_insert(agent);
    mutate_neuron_insert(agent);
    mutate_neuron_insert(agent);
    mutate_neuron_insert(agent);*/


    REPEAT(i, 100,
           mutate_neuron_insert(agent);
           mutate_link_add(agent);
           mutate_link_toggle(agent);
           mutate_link_shift(agent, 0.2);

           print_agent(agent);
        
        printf("\n");
    );

    //insert(&agent->links, new_Link(agent->inputNeurons[1], agent->inputNeurons[0], 0, 0));
    //insert(&agent->neurons, new_BasicNeuron(1));

    if (!check_agent(agent))
    {
        printf("AGENT CORRUPTED!\n");
    }

    return EXIT_SUCCESS;
}