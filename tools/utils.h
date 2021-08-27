#ifndef UTILS_H
#define UTILS_H

#include <time.h>
#include <stdio.h>

#define MCHECK(ptr, ret)								\
	do {												\
		if (ptr == NULL)								\
		{												\
			perror("malloc error");						\
			return ret;									\
		}												\
	} while (0);

#define TIMER(timer, code)								\
	do {												\
		int timer = clock();							\
		code											\
		timer = clock() - timer;						\
	} while (0);

#define REPEAT(iter, nb, code)							\
	do {												\
		for (unsigned int iter = 0; iter < nb; iter++)	\
		{												\
			code										\
		}												\
	} while (0);

#define print(string, ...)								\
	do {												\
		char str_to_print[] = string;					\
		char* str_full = malloc(strlen(str_to_print)+2);\
		MCHECK(str_full, );								\
		strcpy(str_full, str_to_print);					\
		strncat(str_full, "\n\0", 2);					\
		printf(str_full, __VA_ARGS__);					\
		free(str_full);									\
	} while (0);

_Bool isInArray(void* ptr, void* _array[], unsigned int size);

#endif // !UTILS_H
