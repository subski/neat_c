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
    print_link_id_matrix(15);
    Agent* agent;
    Agent* a1, * a2, * a3;

    agent = new_BasicAgent(3, 2);
    agent = (Agent*) cy_insert(&Population, agent);

    agent = new_BasicAgent(3, 2);
    agent = (Agent*) cy_insert(&Population, agent);

    agent = new_BasicAgent(3, 2);
    agent = (Agent*) cy_insert(&Population, agent);

    a1 = agent;

    agent = new_BasicAgent(3, 2);
    mutate_neuron_insert(agent);
    mutate_neuron_insert(agent);
    agent = (Agent*) cy_insert(&Population, agent);

    a2 = agent;

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

    a3 = agent;

    agent = (Agent*) cy_insert(&Population, agent);

    agent = (Agent*) cy_insert(&Population, agent_clone(agent));

    agent = (Agent*) cy_insert(&Population, agent_clone(agent));

    
    // Specie s = { 0, NULL, NULL };
    // cy_insert(&s.specimens, a1);
    // cy_insert(&s.specimens, a2);
    // cy_insert(&s.specimens, a3);

    // printf("a1:\n");
	// print_agent(a1);
	// printf("a2:\n");
	// print_agent(a2);
	// printf("a3:\n");
	// print_agent(a3);

    // s.centroid = CalculateCentroidAgent(s.specimens);
    
	// printf("c:\n");
    // print_agent(s.centroid);

    // printf("Distance A1-C : %lf\n", agent_euclidean_distance(a1, s.centroid));
    // printf("Distance A2-C : %lf\n", agent_euclidean_distance(a2, s.centroid));
    // printf("Distance A3-C : %lf\n", agent_euclidean_distance(a3, s.centroid));

    // printf("Distance A3-A1 : %lf\n", agent_euclidean_distance(a3, a1));
    // printf("Distance A3-A2 : %lf\n", agent_euclidean_distance(a3, a2));

    // printf("Distance A1-A2 : %lf\n", agent_euclidean_distance(a1, a2));

    // free_specie(&s);

    kmeans_init(Population, &Species, 3);
    kmeans_run(Population, Species);

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