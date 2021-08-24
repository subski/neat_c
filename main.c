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

int main(void)
{
	pcg32_srandom(time(NULL), &printf);

	Agent* agent = createBasicAgent();

	clist* l = NULL;
	INSERT_DATA(&l, int, 1);
	INSERT_DATA(&l, int, 2);
	INSERT_DATA(&l, int, 3);
	INSERT_DATA(&l, int, 4);
	INSERT_DATA(&l, int, 5);
	INSERT_DATA(&l, int, 6);

	for (int i=0;i<100;i++)
		printf("%d\n", *(int*)random(l));

	free_agent(&agent);

	clean(&P_LINK);
	clean(&P_NEURON);
	clean(&P_AGENT);

	system("pause");
	return EXIT_SUCCESS;
}