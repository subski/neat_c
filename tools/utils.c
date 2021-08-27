#include "tools/utils.h"

#include <stdbool.h>

_Bool isInArray(void* ptr, void* _array[], unsigned int size)
{
	for (unsigned int i = 0; i < size; i++)
	{
		if (_array[i] == ptr)
			return true;
	}

	return false;
}