#pragma once

#include <memory.h>						// memcpy

struct elliptic_curve_jacobian {
	uint_32 x[_WORD_32_];
	uint_32 y[_WORD_32_];
	uint_32 z[_WORD_32_];
	char flag;
};
typedef struct elliptic_curve_jacobian ECJ;

int  Check_Num0								(cuint32* input);

void ECDBL_Jacobian							(ECJ* input,									ECJ* output);

void ECADD_Jacobian							(ECJ* input1,		ECJ* input2,				ECJ* output);

void Left_To_Right_Binary_MUL_Jacobian		(ECJ* input,		cuint32* inconst,			ECJ* output);

void ECADD_Jacobian_INPUT3					(cuint32* inX,	cuint32* inY,	cuint32* inZ,	ECJ* input2,	ECJ* output);