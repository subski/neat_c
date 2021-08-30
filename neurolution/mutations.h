#ifndef MUTATIONS_H
#define MUTATIONS_H

typedef struct Agent Agent;

bool mutate_link_add   (Agent* agent);

void mutate_link_toggle(Agent* agent);

void mutate_link_shift (Agent* agent, double shift);

bool mutate_node_add   (Agent* agent);

void mutate_node_toggle(Agent* agent);

#endif // !MUTATIONS_H
