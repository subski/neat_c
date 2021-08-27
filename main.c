#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include <Windows.h>

#include "tools/utils.h"
#include "tools/pcg_basic.h"
#include "tools/malloc_dbg.h"

#include "data_structures/clist.h"
#include "data_structures/pool.h"
#include "data_structures/vector.h"

#include "neurolution/env_settings.h"
#include "neurolution/neuron.h"
#include "neurolution/agent.h"
#include "neurolution/neurolution.h"
#include "neurolution/mutations.h"


void onExit(void)
{
	clean(&P_LINK);
	clean(&P_NEURON);
	clean(&P_AGENT);

#if USE_DEBUG_MALLOC
	printLeaks();
#endif
	system("pause");
}

struct pair
{
	int item1;
	int item2;
};

struct pair get_pair(int N[], int count, int index)
{
	int d = 1;
	while (1)
	{
		if (index < count - d)
			return (struct pair) { N[index], N[index + d] };
		index -= (count - d++);
	}
}

int main(void)
{
	atexit(&onExit);
	pcg32_srandom((unsigned long long)time(NULL), (unsigned long long) & printf);


	//vector v = vec_init(sizeof(unsigned short), 0);


	// Vector tests

	vector v = vec_init(sizeof(unsigned short), 4);

	((unsigned short*)v.start)[0] = 50;
	((unsigned short*)v.start)[1] = 100;
	((unsigned short*)v.start)[2] = 200;
	((unsigned short*)v.start)[3] = 300;

	unsigned short t = 400;

	vec_push(&v, &t);


	unsigned short* iter = v.start;
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

	// Mutations tests

	//Agent* agent = createBasicAgent();

	//REPEAT(i, 20,

	//	mutate_link_add(agent);

	//ITER_V(agent->neurons, neuron_node, neuron, Neuron*,
	//	ITER_V(neuron->links, link_node, link, Link*,
	//		print("%d -> %d", link->source->id, neuron->id);
	//);
	//);

	//print("");

	//);
	//free_agent(&agent);

	return EXIT_SUCCESS;
}