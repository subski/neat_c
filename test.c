#include <stdlib.h>
#include <stdio.h>
#include "pool.h"
#include "neuron.h"

int _main(void)
{
	return EXIT_SUCCESS;
}

void test_vector()
{
	vector v = vec_init(sizeof(unsigned short), 0);
	unsigned short* iter;

	unsigned short a = 40;

	vec_push(&v, &a);
	a = 50;
	vec_push(&v, &a);
	a = 60;
	vec_push(&v, &a);
	a = 70;
	vec_push(&v, &a);
	a = 80;
	vec_push(&v, &a);

	vec_mresize(&v, 4);

	a = 90;
	vec_push(&v, &a);

	vec_mresize(&v, 400);

	a = 100;
	vec_push(&v, &a);

	iter = v.start;
	REPEAT(i, v.count,
		print("%d", iter[i]);
	);

	vec_mresize(&v, 4);

	((unsigned short*)v.start)[0] = 50;
	((unsigned short*)v.start)[1] = 100;
	((unsigned short*)v.start)[2] = 200;
	((unsigned short*)v.start)[3] = 300;

	unsigned short t = 400;

	vec_push(&v, &t);


	iter = v.start;
	REPEAT(i, v.count,
		print("%d", iter[i]);
	);

	unsigned short* ptr;

	vec_pop(&v, &ptr);

	print("%d", *ptr);

	free(ptr);

	iter = v.start;
	REPEAT(i, v.count,
		print("%d", iter[i]);
	);

	free(v.start);
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

void test_vec_agents(void)
{
	vector agents = new_vector(sizeof(Agent*), 200, 0);

	Agent* tmp;
	REPEAT(i, 200,
		   tmp = new_BasicAgent();
	tmp->fitness = i;
	vec_insert(&agents, &tmp, i);
	);

	Agent* a;

	REPEAT(j, 200,
		   REPEAT(i, 200,
		   a = *(Agent**)vec_get(&agents, i);
	mutate_neuron_insert(a);
	mutate_link_add(a);
	mutate_link_toggle(a);
	mutate_link_shift(a, 0.2);
	);
	);

	REPEAT(i, 200,
		   a = *(Agent**)vec_get(&agents, i);
	print_agent(a);
	);

	REPEAT(i, 200,
		   free_agent(vec_get(&agents, i));
	);

	free_vector(&agents);

	return 0;
}