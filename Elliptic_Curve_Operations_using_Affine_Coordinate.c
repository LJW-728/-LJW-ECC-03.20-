#include "Basic_Definitions.h"
#include "Prime_Field_Arithmetic.h"
#include "Elliptic_Curve_Operations_using_Affine_Coordinate.h"

void ECADD_Affine							(ECA* input1,		ECA* input2,				ECA* output)
{
	uint_32 temp[_WORD_32_]		= { 0, };
	uint_32 inverse[_WORD_32_]	= { 0, };
	uint_32 lambda[_WORD_32_]	= { 0, };

	uint_32 inX1[_WORD_32_] = { 0, }, inY1[_WORD_32_] = { 0, };
	uint_32 inX2[_WORD_32_] = { 0, }, inY2[_WORD_32_] = { 0, };
	uint_32 outX[_WORD_32_] = { 0, }, outY[_WORD_32_] = { 0, };

	memcpy(inX1, input1->x, 8 * sizeof(uint_32)); memcpy(inY1, input1->y, 8 * sizeof(uint_32));
	memcpy(inX2, input2->x, 8 * sizeof(uint_32)); memcpy(inY2, input2->y, 8 * sizeof(uint_32));

	BIGNUM_SUB_64_RE			(inX2,		inX1,		temp);			// x2-x1
	Binary_Alg_Inversion_Alg	(temp,		inverse);					// (x2-x1)^-1
	BIGNUM_SUB_64_RE			(inY2,		inY1,		temp);			// y2-y1
	BIGNUM_MUL_OS_64_RE			(temp,		inverse,	lambda);		// (y2-y1) * (x2-x1)^-1		= r

	BIGNUM_MUL_OS_64_RE			(lambda,	lambda,		temp);			// r^2
	BIGNUM_SUB_64_RE			(temp,		inX1,		temp);			// r^2-x1
	BIGNUM_SUB_64_RE			(temp,		inX2,		outX);			// r^2-x1-x2				= x3

	BIGNUM_SUB_64_RE			(inX1,		outX,		temp);			// x1-x3
	BIGNUM_MUL_OS_64_RE			(lambda,	temp,		temp);			// r * (x1-x3)
	BIGNUM_SUB_64_RE			(temp,		inY1,		outY);			// r * (x1-x3) -y1			= y3

	memcpy(output->x, outX, 8 * sizeof(uint_32)); 
	memcpy(output->y, outY, 8 * sizeof(uint_32));
}

void ECDBL_Affine							(ECA* input,									ECA* output)
{
	uint_32 temp[_WORD_32_]		= { 0, };
	uint_32 inverse[_WORD_32_]	= { 0, };
	uint_32 lambda[_WORD_32_]	= { 0, };

	uint_32 inX1[_WORD_32_] = { 0, }, inY1[_WORD_32_] = { 0, };
	uint_32 outX[_WORD_32_] = { 0, }, outY[_WORD_32_] = { 0, };

	uint_32 T2[_WORD_32_] = { 0, }; T2[0] = 0X00000002;
	uint_32 T3[_WORD_32_] = { 0, }; T3[0] = 0X00000003;

	memcpy(inX1, input->x, 8 * sizeof(uint_32)); memcpy(inY1, input->y, 8 * sizeof(uint_32));

	BIGNUM_MUL_OS_64_RE			(inY1,		T2,			temp);			// 2 * y1
	Binary_Alg_Inversion_Alg	(temp,		inverse);					// (2 * y1)^-1
	BIGNUM_MUL_OS_64_RE			(inX1,		inX1,		temp);			// x1^2
	BIGNUM_MUL_OS_64_RE			(temp, 		T3,			temp);			// 3 * x1^2
	BIGNUM_SUB_64_RE			(temp,		T3,			temp);			// 3 * x1^2 - 3
	BIGNUM_MUL_OS_64_RE			(temp,		inverse,	lambda);		// (3 * x1^2 - 3) * (2 * y1)^-1		= r

	BIGNUM_MUL_OS_64_RE			(lambda,	lambda,		temp);			// r^2
	BIGNUM_SUB_64_RE			(temp,		inX1,		temp);			// r^2 - x1
	BIGNUM_SUB_64_RE			(temp,		inX1,		outX);			// r^2 - 2 * x1						= x3

	BIGNUM_SUB_64_RE			(inX1,		outX,		temp);			// x1 - x3
	BIGNUM_MUL_OS_64_RE			(lambda,	temp,		temp);			// r * (x1 - x3)
	BIGNUM_SUB_64_RE			(temp,		inY1,		outY);			// r * (x1 - x3) - y1				= y3

	memcpy(output->x, outX, 8 * sizeof(uint_32)); 
	memcpy(output->y, outY, 8 * sizeof(uint_32));
}

void Left_To_Right_Binary_MUL_Affine		(ECA* input,		cuint32* inconst,			ECA* output)
{
	ECA Q; Q.flag = 1;
	ECA P; P.flag = 0;
	uint_32 bit;

	memcpy(P.x, input->x, 8 * sizeof(uint_32));
	memcpy(P.y, input->y, 8 * sizeof(uint_32));
	
	for (int cnt_i = _WORD_32_ - 1; cnt_i >= 0; cnt_i--)
	{
		for (int cnt_j = 31; cnt_j >= 0; cnt_j--)
		{
			bit = (inconst[cnt_i] >> cnt_j) & 0x1;

			if (Q.flag == 0)
				ECDBL_Affine(&Q, &Q);

			if (bit == 1) 
			{
				if (Q.flag == 1)
				{
					memcpy(Q.x, P.x, 8 * sizeof(uint_32));
					memcpy(Q.y, P.y, 8 * sizeof(uint_32));
					Q.flag = 0;
				}
				else
					ECADD_Affine(&Q, &P, &Q);
			}
		}
	}
	memcpy(output->x, Q.x, 8 * sizeof(uint_32));
	memcpy(output->y, Q.y, 8 * sizeof(uint_32));
}

void Left_To_Right_Binary_MUL_Affine2		(ECA* input,		cuint32* inconst,			ECA* output)
{
	ECA Q; Q.flag = 1;
	ECA P; P.flag = 0;
	uint_32 bit;

	memcpy(P.x, input->x, 8 * sizeof(uint_32));
	memcpy(P.y, input->y, 8 * sizeof(uint_32));

	int cnt_i;
	int cnt_j;
	int A= _WORD_32_ - 1;
	int B= 31;

	for (cnt_i = _WORD_32_ - 1; cnt_i >= 0; cnt_i--)
	{
		A--;
		for (cnt_j = 31; cnt_j >= 0; cnt_j--)
		{
			B--;
			bit = (inconst[cnt_i] >> cnt_j) & 0x1;

			if (bit == 1)
			{
				memcpy(Q.x, P.x, 8 * sizeof(uint_32));
				memcpy(Q.y, P.y, 8 * sizeof(uint_32));
				Q.flag = 0;
				break;
			}
		}
		if (Q.flag == 0)
			break;
		B = 31;
	}

	for (cnt_j = B; cnt_j >= 0; cnt_j--)
	{
		bit = (inconst[cnt_i] >> cnt_j) & 0x1;

		ECDBL_Affine(&Q, &Q);

		if (bit == 1)
			ECADD_Affine(&Q, &P, &Q);
	}

	for (cnt_i = A; cnt_i >= 0; cnt_i--)
	{
		for (cnt_j = 31; cnt_j >= 0; cnt_j--)
		{
			bit = (inconst[cnt_i] >> cnt_j) & 0x1;

			ECDBL_Affine(&Q, &Q);

			if (bit == 1)
				ECADD_Affine(&Q, &P, &Q);
		}
	}

	memcpy(output->x, Q.x, 8 * sizeof(uint_32));
	memcpy(output->y, Q.y, 8 * sizeof(uint_32));
}

void Right_To_Left_Binary_MUL_Affine		(ECA* input,		cuint32* inconst,			ECA* output)
{
	ECA Q; Q.flag = 1;
	ECA P; P.flag = 0;
	uint_32 bit;

	memcpy(P.x, input->x, 8 * sizeof(uint_32));
	memcpy(P.y, input->y, 8 * sizeof(uint_32));

	for (int cnt_i = 0; cnt_i < _WORD_32_; cnt_i++)
	{
		for (int cnt_j = 0; cnt_j < 32; cnt_j++)
		{
			bit = (inconst[cnt_i] >> cnt_j) & 0x1;

			if (bit == 1)
			{
				if (Q.flag == 1)
				{
					memcpy(Q.x, P.x, 8 * sizeof(uint_32));
					memcpy(Q.y, P.y, 8 * sizeof(uint_32));
					Q.flag = 0;
				}
				else
					ECADD_Affine(&Q, &P, &Q);
			}

			ECDBL_Affine(&P, &P);
		}
	}

	memcpy(output->x, Q.x, 8 * sizeof(uint_32));
	memcpy(output->y, Q.y, 8 * sizeof(uint_32));
}