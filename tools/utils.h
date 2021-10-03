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

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

// Console prompt helpers
#define NEWLINE() printf("\n");

#define ColorReset() printf("\033[0m");
#define ColorBlack() printf("\033[0;30m");
#define ColorRed() printf("\033[0;31m");
#define ColorGreen() printf("\033[0;32m");
#define ColorYellow() printf("\033[0;33m");
#define ColorBlue() printf("\033[0;34m");
#define ColorPurple() printf("\033[0;35m");
#define ColorCyan() printf("\033[0;36m");
#define ColorWhite() printf("\033[0;37m");

// Path to the binary folder
extern char BIN_PATH[];

/*
	Extract the binary folder path from the executable path. 
*/
void setup_binpath(char* path);

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
