#pragma once

#include <memory.h>						// memcpy

struct elliptic_curve_affine {
	uint_32 x[_WORD_32_];
	uint_32 y[_WORD_32_];
	char flag;
};
typedef struct elliptic_curve_affine ECA;

void ECADD_Affine							(ECA* input1,		ECA* input2,				ECA* output);

void ECDBL_Affine							(ECA* input,									ECA* output);

void Left_To_Right_Binary_MUL_Affine		(ECA* input,		cuint32* inconst,			ECA* output);
void Left_To_Right_Binary_MUL_Affine2		(ECA* input,		cuint32* inconst,			ECA* output);

void Right_To_Left_Binary_MUL_Affine		(ECA* input,		cuint32* inconst,			ECA* output);