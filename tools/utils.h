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

/*
	Check if a given pointer is in an array.
*/
bool isPtrInArray(void* ptr, void* _array[], uint32_t arraySize);

/*
	Return the absolute value of a double.
*/
double double_abs(double x);

#endif // !UTILS_H
