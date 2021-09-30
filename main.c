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
#include "neurolution/kmeans.h"

void onStart(int argc, char* argv[]);
void onExit(void);

bool test(int argc, char* argv[])
{
    Agent *a1, *a2, *a3, *a4;

    a1 = new_BasicAgent(3, 2);
    cy_insert(&Population, a1);
    a2 = new_BasicAgent(3, 2);
    cy_insert(&Population, a2);

    a3 = new_BasicAgent(3, 2);
    mutate_neuron_insert(a3);
    mutate_neuron_insert(a3);
    mutate_neuron_insert(a3);
    cy_insert(&Population, a3);

    a4 = cy_insert(&Population,  agent_clone(a3));

    kmeans_init(Population, &Species, 2);
    kmeans_run(Population, Species);
    // printf("A1-A2: %lf\n", distance(a1, a2, 1, 1));
    // printf("A1-A3: %lf\n", distance(a1, a3, 1, 1));
    // printf("A1-A4: %lf\n", distance(a1, a4, 1, 1));
    // printf("\n");

    // printf("A2-A3: %lf\n", distance(a2, a3, 1, 1));
    // printf("A2-A4: %lf\n", distance(a2, a4, 1, 1));
    // printf("\n");

    // printf("A3-A4: %lf\n", distance(a3, a4, 1, 1));
    // printf("\n");
 
    return false; // continue the program?

    Agent* agent;

    agent = new_BasicAgent(3, 2);
    agent = (Agent*) cy_insert(&Population, agent);

    agent = new_BasicAgent(3, 2);
    agent = (Agent*) cy_insert(&Population, agent);

    agent = new_BasicAgent(3, 2);
    agent = (Agent*) cy_insert(&Population, agent);


    agent = new_BasicAgent(3, 2);
    mutate_neuron_insert(agent);
    mutate_neuron_insert(agent);
    agent = (Agent*) cy_insert(&Population, agent);


    agent = (Agent*) cy_insert(&Population, agent_clone(agent));
    mutate_link_add(agent);
    
    agent = (Agent*) cy_insert(&Population, agent_clone(agent));
    mutate_link_add(agent);

    agent = new_BasicAgent(3, 2);
    mutate_neuron_insert(agent);
    mutate_neuron_insert(agent);
    mutate_neuron_insert(agent);
    mutate_neuron_insert(agent);
    mutate_link_shift(agent, 0.2);
    mutate_link_shift(agent, 0.2);
    mutate_link_shift(agent, 0.2);
    mutate_link_shift(agent, 0.2);
    mutate_link_toggle(agent);
    mutate_link_toggle(agent);
    mutate_link_toggle(agent);

    agent = (Agent*) cy_insert(&Population, agent);

    agent = (Agent*) cy_insert(&Population, agent_clone(agent));

    agent = (Agent*) cy_insert(&Population, agent_clone(agent));
    
    return false; // continue the program?
}

int main(int argc, char* argv[])
{
    onStart(argc, argv);
    if (!test(argc, argv)) return 0;
    
    // evolve();

    return EXIT_SUCCESS;
}

#ifdef _WIN32
#include <sys\timeb.h> 
struct timeb progStart, progEnd;
#endif // !_WIN32

void onStart(int argc, char* argv[])
{
    printf(
" _      _____ ____  _____     ____ \n"
"/ \\  /|/  __//  _ \\/__ __\\   /   _\\\n"
"| |\\ |||  \\  | / \\|  / \\     |  /  \n"
"| | \\|||  /_ | |-||  | |     |  \\__\n"
"\\_/  \\|\\____\\\\_/ \\|  \\_/_____\\____/\n"
"                        \\____\\ \n\n");
    // Add function on program exit event
    atexit(&onExit);
    // Setup the random number generator
    pcg32_srandom((unsigned long long)time(NULL), (unsigned long long) & printf);

    // Get the program binary path from the program arguments
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
    // Save the time when the program starts to get the execution time later
    ftime(&progStart);
#endif // !_WIN32

    // Handle program's argument
    for (int i=1; i<argc; i++)
    {
        switch (argv[i][1])
        {
            // Plot agent from file
            case 'p':
                printf("Plotting agent from file: %s\n", argv[i+1]);
#ifdef _WIN32
                char cmd[255] = "python ";
#else
                char cmd[255] = "python3 ";
#endif // !_WIN32
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
#endif  // !USE_DEBUG_MALLOC

#ifdef _WIN32

    ftime(&progEnd);
    
    printf("\n\n---------------------------------------------------------------------------------------------------------------------\n");
    printf("Program ended in %.3f seconds.\n", (1000 * (progEnd.time - progStart.time) + (progEnd.millitm - progStart.millitm)) / 1000.f);
    //system("pause");

    // Clear temporary files created when ploting agents
    char cmd[255] = "start cmd /c del ";
    strcat(cmd, BIN_PATH);
    strcat(cmd, "{*}");
    system(cmd);

#endif // !_WIN32
}