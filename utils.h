#pragma once
#ifndef UTILS_H
#define UTILS_H

#define mcheck(ptr, ret)				\
	if (ptr == NULL)			\
	{							\
		perror("malloc error");	\
		return ret;			\
	}

#endif
