#pragma once
#ifndef UTILS_H
#define UTILS_H

#define mcheck(ptr, ret)		\
	if (ptr == NULL)			\
	{							\
		perror("malloc error");	\
		return ret;				\
	}

#ifndef _STDINT

typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

#endif // !_STDINT
#endif // !UTILS_H
