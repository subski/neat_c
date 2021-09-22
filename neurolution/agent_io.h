#ifndef AGENT_IO_H
#define AGENT_IO_H

#include "neurolution/agent.h"

/*
    Print an agent's connections in the standard output.
*/
void print_agent(Agent* agent);

/*
    Save an agent in a given file.
*/
bool save_agent(char filename[], Agent* agent);

/*
    Load an agent from a given file.
*/
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
        Store the agent into a file '<unique_id>'.
        Calls the command 'python plot.py <unique_id>'.
        Returns the python process id with 'tasklist'.
        To close a plot window, use:
            char cmd[100] = "taskkill /F /PID ";    
            strcat(cmd, <PID>);
            system(cmd);        
*/
int plot_agent(Agent* agent, char pid_str[]);

/*
    Display an agent's graph and apply mutations to it via the command line.
    Commands:
        'l': new link
        't': toggle link
        's': link shift
        'n': new neuron
*/
void interactive_agent_plot(uint32_t inputSize, uint32_t outputSize);

#endif