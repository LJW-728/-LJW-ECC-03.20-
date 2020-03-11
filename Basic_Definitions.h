#pragma once

#include <stdio.h>
#include <stdint.h>				// int64_t

#define _SUCCESS_		1
#define _FAILURE_		0
#define _WORD_32_		8
#define _INFINITY_		1

typedef unsigned long long		uint_64;
typedef const unsigned int		cuint32;
typedef unsigned int			uint_32;
typedef const unsigned char		cuchar8;
typedef unsigned char			uchar_8;

extern uint_32  P_256[8];
extern uint_32  P_256_2C[8];	// two's complement
extern uint_32  P_256_D2[8];