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

#include <sys\timeb.h> 

struct timeb progStart, progEnd;
void onExit(void)
{
    free_neurolution();

#if USE_DEBUG_MALLOC
    printLeaks();
#endif

    ftime(&progEnd);
    
    printf("\n\n---------------------------------------------------------------------------------------------------------------------\n");
    printf("Program ended in %.3f seconds.\n", (1000 * (progEnd.time - progStart.time) + (progEnd.millitm - progStart.millitm)) / 1000.f);
    system("pause");
}

void onStart(void)
{
    atexit(&onExit);
    pcg32_srandom((unsigned long long)time(NULL), (unsigned long long) & printf);

    ftime(&progStart);
}

bool test()
{
    int co1, co2;
    
    for (int i = 1; i < 11; i++)
    {
        idToPair(i, &co1, &co2);
        printf("%d: (%d, %d) ", i, co1, co2);
        print("%d: (%d, %d)", pairToId(co1, co2), co1, co2);
    }

    return true;
}

int main(void)
{
    onStart();

    //if (!test()) return 0;

    Agent* agent = new_BasicAgent();

    mutate_neuron_insert(agent);
    mutate_neuron_insert(agent);
    mutate_neuron_insert(agent);
    mutate_neuron_insert(agent);
    mutate_neuron_insert(agent);
    mutate_neuron_insert(agent);


    REPEAT(i, 1,
        //mutate_link_add(agent);
        //mutate_link_toggle(agent);
        //mutate_link_shift(agent, 0.2);

        ITER_V(agent->neurons, neuron_node, neuron, Neuron*,
            ITER_V(neuron->links, link_node, link, Link*,
                print("%d -> %d [%.2lf, %d]", link->source->id, neuron->id, link->weight, link->enabled);
            );
        );
        
        print("Neurons: %d | Links: %d", len(agent->neurons), len(agent->links));
        
        printf("\n");
    );

    //insert(&agent->links, new_Link(agent->inputNeurons[1], agent->inputNeurons[0], 0, 0));
    //insert(&agent->neurons, new_BasicNeuron(1));

    if (!check_agent(agent))
    {
        print("AGENT CORRUPTED!");
    }

    free_agent(&agent);

    return EXIT_SUCCESS;
}