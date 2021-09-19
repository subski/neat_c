#ifndef AGENT_IO_H
#define AGENT_IO_H

#include "neurolution/agent.h"

void print_agent(Agent* agent);

bool save_agent(char filename[], Agent* agent);

Agent* load_agent(char filename[]);

int plot_agent(Agent* agent, char* argv[], char pid_str[]);

#endif