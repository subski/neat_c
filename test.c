#include <stdlib.h>
#include <stdio.h>
#include "pool.h"
#include "neuron.h"

int _main(void)
{
	
	return EXIT_SUCCESS;
}

void test_pool()
{
	pool* neuron_pool = NULL;
	clist* n = NULL;

	for (int j = 0; j < 100; j++)
	{
		for (int i = 0; i < 1000; i++)
		{
			insert(&n, request(&neuron_pool, sizeof(Neuron)));
		}
		printf("pool size: %d\n", len(neuron_pool));
		printf("n list size: %d\n", len(n));

		for (int i = 0; i < 500; i++)
		{
			clist* node = n->next;
			n->next = node->next;
			pfree(&neuron_pool, &node->data, 0);
			free(node);
		}

		printf("pool size: %d\n", len(neuron_pool));
		printf("n list size: %d\n", len(n));

		for (int i = 0; i < 250; i++)
		{
			insert(&n, request(&neuron_pool, sizeof(Neuron)));
		}

		printf("pool size: %d\n", len(neuron_pool));
		printf("n list size: %d\n", len(n));

		clean(&n);
		clean(&neuron_pool);
	}
}