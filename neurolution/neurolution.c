#include "neurolution/neurolution.h"

#include <math.h>

#include "data_structures/pool.h"
#include "neurolution/env_settings.h"

pool* P_AGENT  = NULL;
pool* P_NEURON = NULL;
pool* P_LINK   = NULL;

unsigned int NeuronCount = INPUT_SIZE + OUTPUT_SIZE;

// TODO: neuron mutation history list.

void idToPair(uint32_t id, uint32_t* p1, uint32_t* p2)
{
	uint32_t r_id;

	if (id % 2 == 0)
	{
		r_id = id / 2;
	}
	else
	{
		r_id = (id + 1) / 2;
	}

	float n = 0.5f + sqrtf(0.25f + 2 * r_id);
	float nb = ceilf(n) - 1;

	if (id % 2 == 0)
	{
		*p1 = (int)roundf(nb * (n - nb));
		*p2 = (int)ceilf(n);
	}
	else
	{
		*p1 = (int)ceilf(n);
		*p2 = (int)roundf(nb * (n - nb));
	}
}

uint32_t pairToId(uint32_t p1, uint32_t p2)
{
	return 0;
}
