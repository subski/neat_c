#include "tools/utils.h"

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

#if USE_SDL

void DrawCircle(SDL_Renderer *renderer, int x, int y, int radius)
{
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

#endif // !USE_SDL