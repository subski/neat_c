#include <string.h>

#if USE_SDL
#include <SDL.h>
#endif // !USE_SDL

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
	Population = new_vector(sizeof(Agent*), 9, 0);

    Agent* agent;
    
    agent = new_BasicAgent(3, 2);
    VEC(Population, Agent*, 0) = agent;

    agent = new_BasicAgent(3, 2);
    VEC(Population, Agent*, 1) = agent;

    agent = new_BasicAgent(3, 2);
    VEC(Population, Agent*, 2) = agent;


    agent = new_BasicAgent(3, 2);
    mutate_neuron_insert(agent);
    mutate_neuron_insert(agent);
    VEC(Population, Agent*, 3) = agent;


    VEC(Population, Agent*, 4) = agent_clone(agent);
    mutate_link_add(VEC(Population, Agent*, 4));
    
    VEC(Population, Agent*, 5) = agent_clone(VEC(Population, Agent*, 4));
    mutate_link_add(VEC(Population, Agent*, 5));

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

    VEC(Population, Agent*, 6) = agent;

    VEC(Population, Agent*, 7) = agent_clone(agent);

    VEC(Population, Agent*, 8) = agent_clone(agent);
    
    return false; // continue the program?
}

int main(int argc, char* argv[])
{
    onStart(argc, argv);
    if (!test(argc, argv)) return 0;
    
    evolve();

    return EXIT_SUCCESS;
}

#ifdef _WIN32
#include <sys\timeb.h> 
struct timeb progStart, progEnd;
#endif // !_WIN32

// TODO: add welcolme msg
// TODO: use USE_DEBUG_MALLOC and USE_SDL from CMakeLists.txt
void onStart(int argc, char* argv[])
{
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

#if USE_SDL

    // Init SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Could not init SDL: %s\n", SDL_GetError());
       exit(1);
    }
    // TODO: take cmake project name as window title
    ui_screen = SDL_CreateWindow("NEAT_C",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            UI_WIDTH, UI_HEIGHT,
            0);
    if(!ui_screen) {
        fprintf(stderr, "Could not create window\n");
        exit(1);
    }
    ui_renderer = SDL_CreateRenderer(ui_screen, -1, SDL_RENDERER_SOFTWARE);
    if(!ui_renderer) {
        fprintf(stderr, "Could not create renderer\n");
        exit(1);
    }

    SDL_SetRenderDrawColor(ui_renderer, 0, 0, 0, 255);

#endif // !USE_SDL

}

void onExit(void)
{
#if USE_SDL

    SDL_DestroyWindow(ui_screen);
    SDL_Quit();

#endif // !USE_SDL

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