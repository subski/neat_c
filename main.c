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

float programTimer;
void onExit(void)
{
	clean(&P_LINK);
	clean(&P_NEURON);
	clean(&P_AGENT);

#if USE_DEBUG_MALLOC
	printLeaks();
#endif
	printf("Program ended in %.3f seconds.\n", (clock() - programTimer) / CLOCKS_PER_SEC);
	system("pause");
}

void onStart(void)
{
	atexit(&onExit);
	pcg32_srandom((unsigned long long)time(NULL), (unsigned long long) & printf);

	programTimer = (float)clock();
}

int main(void)
{
	onStart();

	Agent* agent = createBasicAgent();

	REPEAT(i, 10,
		mutate_link_add(agent);
	mutate_link_toggle(agent);
	mutate_link_shift(agent, 0.2);

	ITER_V(agent->neurons, neuron_node, neuron, Neuron*,
		ITER_V(neuron->links, link_node, link, Link*,
			print("%d -> %d [%lf, %d]", link->source->id, neuron->id, link->weight, link->enabled);
	);
	);

	printf("\n");

	);

	free_agent(&agent);

	return EXIT_SUCCESS;
}