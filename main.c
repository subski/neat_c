#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

#include <time.h>

#include <Windows.h>

#include "utils.h"
#include "env_settings.h"
#include "clist.h"
#include "llist.h"
#include "pool.h"
#include "neuron.h"
#include "agent.h"
#include "neurolution.h"
#include "mutations.h"
#include "pcg_basic.h"
#include "malloc_dbg.h"

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

typedef struct pair
{
	int item1;
	int item2;
} pair;

void print_pair(pair _pair)
{
	printf("%d -> %d\n", _pair.item1, _pair.item2);
}

void print_pairs(pair* rotating_pairs, unsigned int size)
{
	for (unsigned int i = 0; i < size; i++)
	{
		printf("%d -> %d\n", rotating_pairs[i].item1, rotating_pairs[i].item2);
	}
}

pair* goto_schedule(unsigned int n, pair* rotating_pairs, unsigned int size)
{
	if (n == 0 || n > size * 2)
		return;

	pair* new_pairs = malloc(sizeof(pair) * (size + 1));

	if (n > size)
	{
		new_pairs[0].item2 = rotating_pairs[size - (n - size)].item1;
	}
	else
	{
		new_pairs[0].item2 = rotating_pairs[n - 1].item2;
	}

	//for (unsigned int i = 0; i < size; i++)
	//{
	//	new_pairs[i + 1] = (pair){ numbers[2 + i], numbers[size - 1 - i] };
	//}
}

int main(void)
{
	atexit(&onExit);
	pcg32_srandom((unsigned long long)time(NULL), (unsigned long long) & printf);

	/*Agent* agent = createBasicAgent();
	mutate_link_add(agent);
	free_agent(&agent);*/

	unsigned int size = 10;
	unsigned int numbers[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	unsigned int rotating_pairs_length = (size) / 2;
	pair* rotating_pairs = malloc(sizeof(pair) * rotating_pairs_length);
	rotating_pairs[0] = (pair){ numbers[0], numbers[1] };

	for (unsigned int i = 1; i < rotating_pairs_length-1; i++)
	{
		rotating_pairs[i] = (pair){ numbers[i+1], numbers[size - 1 - i] };
	}

	//goto_schedule(i, &locked_pair, rotating_pairs, rotating_pairs_length);
	//print_pair(locked_pair);
	print_pairs(rotating_pairs, rotating_pairs_length);

	free(rotating_pairs);

	return EXIT_SUCCESS;
}