#include "tools/utils.h"

const int ZERO = 0;
const int ONE = 1;

bool isPtrInArray(void* ptr, void* _array[], uint32_t arraySize)
{
	for (uint32_t i = 0; i < arraySize; i++)
	{
		if (_array[i] == ptr)
			return true;
	}

	return false;
}

double double_abs(double x)
{
	if (x < 0) {
		return -x;
	}
	return x;
}