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

// Very ugly way to automatically insert '\n' at the end of printf. Use like printf.
#define print(string, ...)								\
	do {												\
		char str_to_print[] = string;					\
		char* str_full = malloc(strlen(str_to_print)+2);\
		if (str_full == NULL) break;					\
		_Pragma("warning (suppress : 4996)")			\
		strcpy(str_full, str_to_print);					\
		strncat(str_full, "\n\0", 2);					\
		printf(str_full, __VA_ARGS__);					\
		free(str_full);									\
	} while (0);


static int ZERO = 0;
static int ONE = 1;

/*
	Check if a given pointer is in an array.
*/
bool isPtrInArray(void* ptr, void* _array[], uint32_t arraySize);

/*
	Return the absolute value of a double.
*/
double double_abs(double x);

#endif // !UTILS_H
