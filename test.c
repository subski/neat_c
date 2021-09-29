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
		printf("%d\n", iter[i]);
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
		printf("%d\n", iter[i]);
	);

	unsigned short* ptr;

	vec_pop(&v, &ptr);

	printf("%d\n", *ptr);

	free(ptr);

	iter = v.start;
	REPEAT(i, v.count,
		printf("%d\n", iter[i]);
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
			cy_insert(&n, request(&neuron_pool, sizeof(Neuron)));
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
			cy_insert(&n, request(&neuron_pool, sizeof(Neuron)));
		}

		printf("pool size: %d\n", len(neuron_pool));
		printf("n list size: %d\n", len(n));

		cy_clean(&n);
		cy_clean(&neuron_pool);
	}
}

void test_vec_agents(void)
{
	vector agents = new_vector(sizeof(Agent*), 200, 0);

	Agent* tmp;
	REPEAT(i, 200,
		   tmp = new_BasicAgent(INPUT_SIZE, OUTPUT_SIZE);
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

void test_pair(void)
{
	uint32_t p1, p2;
    for (int i=-9; i<10; i++)
    {
        idToPair(i, &p1, &p2);
        printf("%d: (%d, %d)\n", pairToId(p1, p2), p1, p2);
    }

    return false;
}

void test_isPtrInArray(void)
{
	Agent* agent = new_BasicAgent(INPUT_SIZE, OUTPUT_SIZE);

	for (int i = 0; i < INPUT_SIZE; i++)
	{
		printf("%p\n", agent->inputVector.start + (i * sizeof(void*)));
	}
	printf("%p\n", (Neuron*)agent->inputVector.start);

	if (isPtrInArray((Neuron*)agent->inputVector.start, agent->inputVector.start, INPUT_SIZE))
		printf("Correct! Ptr is in fact in the array\n");
}

void test_neuronInInput(void)
{
	Neuron* n;

	for (int i = 0; i < INPUT_SIZE; i++)
	{
		n = *(Neuron**)vec_get(&agent->inputVector, i);
		printf("Neuron %d: %p\n", n->id, n);
	}
	printf("\n");
	for (int i = 0; i < OUTPUT_SIZE; i++)
	{
		n = *(Neuron**)vec_get(&agent->outputVector, i);

		printf("Neuron %d: %p\n", n->id, n);
	}

	printf("\n");
	printf("\n");

	CY_ITER_DATA(agent->neuronList, neuron_node, neuron, Neuron*,
		   printf("Neuron %d: %p\n", neuron->id, neuron);
	);
}

bool test_distance()
{   
    NeuronCount = 10;

    Agent* agent1 = new_BasicAgent(3, 2);
    Agent* agent2 = new_BasicAgent(3, 2);

    for (int i = 0; i < 40; i++)
    {
        mutate_agent(agent1);
        mutate_agent(agent2);
    }

    print_agent(agent1);
    print_agent(agent2);

    if (!check_agent(agent1) || !check_agent(agent2))
    {
        printf("AGENT CORRUPTED!\n");
    }

    double d = distance(agent1, agent2, 1.0, 1.0);

    printf("Distance: %lf\n", d);

    free_agent(&agent1);
    free_agent(&agent2);

    return false;
}

bool test_io()
{
    Agent* agent1;
    
    agent1 = load_agent("test.genome");

    print_agent(agent1);

    // throw segmentation fault because we don't initialize inputVector when loading the agent.
    if (!check_agent(agent1))
    {
        printf("AGENT CORRUPTED!\n");
    }

    free_agent(&agent1);

    return false;
}

bool test_crossover()
{
	NeuronCount = 10;

    Agent* agent1 = new_BasicAgent(3, 2);
    Agent* agent2 = new_BasicAgent(3, 2);
    
    mutate_neuron_insert(agent1);
    cy_insert(&agent2->neuronList, new_BasicNeuron(9));
    
    printf("Distance a1 & a2: %lf\n", distance(agent1, agent2, 1.0, 1.0));

    Agent* unwanted_child = crossOver(agent1, agent2);

    char pid[255];
    plot_agent(unwanted_child, pid);

    system("Pause");

    char cmd[255];
    strcpy(cmd, "start taskkill /F /PID ");    
    strcat(cmd, pid);
    system(cmd);

    free_agent(&unwanted_child);
    free_agent(&agent1);
    free_agent(&agent2);
    return false;
}