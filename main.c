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

static pool* AGENTPOOL = NULL;
static pool* NEURONPOOL = NULL;
static pool* LINKPOOL = NULL;

int main(void)
{

	system("pause");
	return EXIT_SUCCESS;
}