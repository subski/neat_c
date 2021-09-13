#include "tools/utils.h"

bool isPtrInArray(void* ptr, void* _array[], uint32_t arraySize)
{
	for (uint32_t i = 0; i < arraySize; i++)
	{
		if (_array[i] == ptr)
			return true;
	}

	return false;
}
