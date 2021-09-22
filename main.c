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
    NeuronCount = 10;

    Agent* agent1 = new_BasicAgent(3, 2);
    Agent* agent2 = new_BasicAgent(3, 2);
    
    mutate_neuron_insert(agent1);
    insert(&agent2->neuronList, new_BasicNeuron(9));
    
    printf("Distance a1 & a2: %lf\n", distance(agent1, agent2, 1.0, 1.0));

    Agent* unwanted_child = crossOver(agent1, agent2);

    char pid[255];
    plot_agent(unwanted_child, pid);

    system("Pause");

    char cmd[255];
    strcpy(cmd, "start taskkill /F /PID ");    
    strcat(cmd, pid);
    system(cmd);

    free_agent(&unwanted_child);
    free_agent(&agent1);
    free_agent(&agent2);
    return false;
}

int main(int argc, char* argv[])
{
    onStart(argc, argv);
    if (!test(argc, argv)) return 0;
    
    evolve();

    Agent* agent = * (Agent**) Population.start;
    save_agent("example_agent.g", agent);

#if _WIN32

    char inp = '\0';
    char pid[32];
    char cmd[100];
    while (inp != 'q')
    {
        plot_agent(agent, pid);
        printf(">");
        scanf(" %c", &inp);

        strcpy(cmd, "start taskkill /F /PID ");    
        strcat(cmd, pid);
        system(cmd); 

        switch (inp)
        {
            case 'l':
                mutate_link_add(agent);
                break;
            case 's':
                mutate_link_shift(agent, 0.2);
                break;
            case 't':
                mutate_link_toggle(agent);
                break;
            case 'n':
                mutate_neuron_insert(agent);
                break;
            default:
            break;
        }
    }
    
#endif // !_WIN32

    return EXIT_SUCCESS;
}

#ifdef _WIN32
#include <sys\timeb.h> 
struct timeb progStart, progEnd;
#endif // !_WIN32

// TODO: add welcolme msg
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
#endif // !_WIN32

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

    char cmd[255] = "start cmd /c del ";
    strcat(cmd, BIN_PATH);
    strcat(cmd, "{*}");
    system(cmd);
#endif // !_WIN32
}