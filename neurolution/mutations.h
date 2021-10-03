#ifndef MUTATIONS_H
#define MUTATIONS_H

typedef struct Agent Agent;

bool mutate_link_add     (Agent* agent);

void mutate_link_toggle  (Agent* agent);

bool mutate_link_shift   (Agent* agent, double shift);

void mutate_neuron_add   (Agent* agent);

bool mutate_neuron_insert(Agent* agent, double (*activationFunc)(double));

void mutate_neuron_toggle(Agent* agent);

void mutate_neuron_bias  (Agent* agent);

void mutate_agent        (Agent* agent);

#endif // !MUTATIONS_H
