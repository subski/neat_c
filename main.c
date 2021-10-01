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

    return true; // continue the program?
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