#ifndef AGENT_IO_H
#define AGENT_IO_H

#include "neurolution/agent.h"

void print_agent(Agent* agent);

bool save_agent(char filename[], Agent* agent);

Agent* load_agent(char filename[]);

/*
    Plot an agent in a separate process and return the PID of the process.
    Args:
        - Agent* agent: target agent
        - char* argv[]: program command line arguments
        - char pid_str[]: give the pid as a string
    Return: PID of the plot process or 0 if no process were created.

    Info:
        (Windows)
        Store the agent into a file 'tmp_agent.g'.
        Calls the command 'python plot.py tmp_agent.g'.
        Returns the python process id with 'tasklist'.
        To close a plot window, use:
            char cmd[100] = "taskkill /F /PID ";    
            strcat(cmd, <PID>);
            system(cmd);        
*/
int plot_agent(Agent* agent, char* argv[], char pid_str[]);

#endif