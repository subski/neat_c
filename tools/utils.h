#ifndef UTILS_H
#define UTILS_H

#define MCHECK(ptr, ret)								\
	do {												\
		if (ptr == NULL)								\
		{												\
			perror("malloc error");						\
			return ret;									\
		}												\
	} while (0);

#define TIMER(timer, code)								\												\
	float timer = clock();								\
	code												\
	timer = (clock() - timer) / CLOCKS_PER_SEC			\

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
		if (str_full == NULL) break;					\
		_Pragma("warning (suppress : 4996)")			\
		strcpy(str_full, str_to_print);					\
		strncat(str_full, "\n\0", 2);					\
		printf(str_full, __VA_ARGS__);					\
		free(str_full);									\
	} while (0);

bool ptrInArray(void* ptr, void* _array[], unsigned int size);

#endif // !UTILS_H
