#ifndef UTILS_H
#define UTILS_H

#define MCHECK(ptr, ret)		\
	if (ptr == NULL)			\
	{							\
		perror("malloc error");	\
		return ret;				\
	}

#endif // !UTILS_H
