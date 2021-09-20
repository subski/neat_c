#include <string.h>

#include "tools/utils.h"
#include "tools/pcg_basic.h"

#include "data_structures/clist.h"
#include "data_structures/pool.h"
#include "data_structures/vector.h"

#include "neurolution/env_settings.h"
#include "neurolution/neuron.h"
#include "neurolution/agent.h"
#include "neurolution/agent_io.h"
#include "neurolution/neurolution.h"
#include "neurolution/mutations.h"

void onStart(int argc, char* argv[]);
void onExit(void);

bool test(int argc, char* argv[])
{
    // NEXT: check todo for 'distance()' and 'crossOver()' functions
    // NEXT: interactive plot mutation on agent
    // NEXT: don't show disabled links in agent plot
    Agent* agent1 = new_BasicAgent(3, 2);

    mutate_neuron_insert(agent1);


#if _WIN32
    char pid_str[32];
    plot_agent(agent1, pid_str);

    system("pause");

    char cmd[100] = "taskkill /F /PID ";    
    strcat(cmd, pid_str);
    system(cmd);    
#endif

    free_agent(&agent1);
    return false;
}

int main(int argc, char* argv[])
{
    onStart(argc, argv);

    if (!test(argc, argv)) return 0;
    
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

#ifdef _WIN32
#include <sys\timeb.h> 
struct timeb progStart, progEnd;
#endif

void onStart(int argc, char* argv[])
{
    atexit(&onExit);
    pcg32_srandom((unsigned long long)time(NULL), (unsigned long long) & printf);

    strcpy(BIN_PATH, argv[0]);
	for (size_t i=strlen(BIN_PATH)-1; i>=0; i--)
	{
		if (BIN_PATH[i] == '/' || BIN_PATH[i] == '\\')
		{
			BIN_PATH[i+1] = '\0';
			break;
		}
	}
    
#ifdef _WIN32
    ftime(&progStart);
#endif

    for (int i=1; i<argc; i++)
    {
        switch (argv[i][1])
        {
            case 'p':
                printf("Plotting agent from file: %s\n", argv[i+1]);
#ifdef _WIN32
                char cmd[255] = "python ";
#else
                char cmd[255] = "python3 ";
#endif
                strcat(cmd, BIN_PATH);
                strcat(cmd, "plot.py ");
                strcat(cmd, argv[i+1]);

                system(cmd);

                exit(0);
            break;
            default:
            break;
        }
    }
}

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
    //system("pause");

    char cmd[255] = "del ";
    strcat(cmd, BIN_PATH);
    strcat(cmd, "{*}");
    system(cmd);
#endif
}