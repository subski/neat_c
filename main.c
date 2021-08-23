#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

#include <Windows.h>

#include "utils.h"
#include "env_settings.h"
#include "clist.h"
#include "llist.h"
#include "pool.h"
#include "neuron.h"
#include "agent.h"
#include "neurolution.h"

int main(void)
{
	clist* agents = NULL;
	for (int i = 0; i < 1000; i++)
	{
		insert(&agents, createBasicAgent());
	}

	clist* tmp;
	for (int i = 0; i < 500; i++)
	{
		tmp = agents->next;
		agents->next = tmp->next;
		free_agent(&tmp->data);
		free(tmp);
	}

	printf("plink %d\n", len(P_LINK));
	printf("pneuron %d\n", len(P_NEURON));
	printf("pagent %d\n", len(P_AGENT));

	for (int i = 0; i < 600; i++)
	{
		insert(&agents, createBasicAgent());
	}

	printf("plink %d\n", len(P_LINK));
	printf("pneuron %d\n", len(P_NEURON));
	printf("pagent %d\n", len(P_AGENT));

	clist* iter = agents;
	do
	{
		free_agent((Agent**)&iter->data);
		next(iter);
	} while (iter != agents);

	clear(&agents);

	clean(&P_LINK);
	clean(&P_NEURON);
	clean(&P_AGENT);

	system("pause");
	return EXIT_SUCCESS;
}