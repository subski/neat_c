#include "tools/utils.h"

#include <math.h>

const int ZERO = 0;
const int ONE = 1;
const int TWO = 2;

char BIN_PATH[255];

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

// TODO: Review the use of calculating pairs id instead of storing them.
// Idea: A [{MAX_NEURONS}x{MAX_NEURONS}] matrix where each element is a {POPULATION_SIZE} array with pointers to links.
void idToPair(int id, uint32_t* p1, uint32_t* p2)
{
	bool invert = false;
	if (id < 0)
	{
		id = -id;
		invert = true;
	}

	#pragma warning (push)
	#pragma warning(disable : 4244)
	float n1 = 0.5f + sqrt(2 * id + 0.25f); // magic formula
	
	uint32_t n1_c = ceilf(n1);
	
	
	uint32_t n2 = (n1_c - 1) * (n1 - (n1_c - 1 ));
	#pragma warning (pop)

	if (!invert)
	{
		*p1 = n1_c;
		*p2 = n2;
	}
	else
	{
		*p1 = n2;
		*p2 = n1_c;
	}
}

int pairToId(uint32_t p1, uint32_t p2)
{
	int invert = 1;
	if (p2 > p1)
	{
		invert = -1;
		int tmp = p1;
		p1 = p2;
		p2 = tmp;		
	}

	return invert * ((p1 * (p1 - 1)) / 2 - (p1 - p2 - 1));
}