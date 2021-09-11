#include "tools/utils.h"

bool ptrInArray(void* ptr, void* _array[], size_t size)
{
	for (uint32_t i = 0; i < size; i++)
	{
		if (_array[i] == ptr)
			return true;
	}

	return false;
}
