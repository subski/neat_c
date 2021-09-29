#ifndef UTILS_H
#define UTILS_H

// Shortcut for checking return value of malloc.
#define MCHECK(ptr, ret)								\
	do {												\
		if (ptr == NULL)								\
		{												\
			perror("malloc error");						\
			return ret;									\
		}												\
	} while (0);

// Mesure time duration of a given code.
#define TIMER(timer, code)								\
	float timer = clock();								\
	code												\
	timer = (clock() - timer) / CLOCKS_PER_SEC; 		\

// Shortcut for a basic FOR loop.
#define REPEAT(iter, nb, code)							\
	do {												\
		for (unsigned int iter = 0; iter < nb; iter++)	\
		{												\
			code										\
		}												\
	} while (0);


extern const int ZERO;
extern const int ONE;
extern const int TWO;

extern char BIN_PATH[];


#if USE_SDL

#include <SDL.h>

#define UI_WIDTH 900
#define UI_HEIGHT 500

SDL_Window* ui_screen;
SDL_Renderer* ui_renderer;

void DrawCircle(SDL_Renderer *renderer, int x, int y, int radius);

#endif // !USE_SDL
/*
	Check if a given pointer is in an array.
*/
bool isPtrInArray(void* ptr, void* _array[], uint32_t arraySize);

/*
	Return the absolute value of a double.
*/
double double_abs(double x);

void idToPair(int id, uint32_t* p1, uint32_t* p2);

int pairToId(uint32_t p1, uint32_t p2);

#endif // !UTILS_H
