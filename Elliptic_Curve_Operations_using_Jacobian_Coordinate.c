#include "Basic_Definitions.h"
#include "Prime_Field_Arithmetic.h"
#include "Elliptic_Curve_Operations_using_Jacobian_Coordinate.h"

int  Check_Num0								(cuint32* input)
{
	for (int cnt_i = 0; cnt_i < _WORD_32_; cnt_i++)
		if (input[cnt_i] != 0)
			return _FAILURE_;

	return _SUCCESS_;
}

void ECDBL_Jacobian							(ECJ* input,									ECJ* output)
{
	uint_32 TEMP1[_WORD_32_]	= { 0, };
	uint_32 TEMP2[_WORD_32_]	= { 0, };
	uint_32 TEMP3[_WORD_32_]	= { 0, };
	uint_32 INVERSE[_WORD_32_]	= { 0, };
	uint_32 LAMBDA[_WORD_32_]	= { 0, };

	uint_32 INX1[_WORD_32_] = { 0, }, INY1[_WORD_32_] = { 0, }, INZ1[_WORD_32_] = { 0, };
	uint_32 OUTX[_WORD_32_] = { 0, }, OUTY[_WORD_32_] = { 0, }, OUTZ[_WORD_32_] = { 0, };

	uint_32 CONST2[_WORD_32_] = { 0, }; CONST2[0] = 0X00000002;
	uint_32 CONST3[_WORD_32_] = { 0, }; CONST3[0] = 0X00000003;

	memcpy(INX1, input->x, _WORD_32_ * sizeof(uint_32));
	memcpy(INY1, input->y, _WORD_32_ * sizeof(uint_32));
	memcpy(INZ1, input->z, _WORD_32_ * sizeof(uint_32));

	if (input->flag == 1)
	{
		memcpy(OUTX, input->x, _WORD_32_ * sizeof(uint_32));
		memcpy(OUTY, input->y, _WORD_32_ * sizeof(uint_32));
		memcpy(OUTZ, input->z, _WORD_32_ * sizeof(uint_32));
		memcpy(output->flag, input->flag, sizeof(char));
		goto EXIT;
	}

	BIGNUM_SQU_OS_64_RE			(INZ1,					TEMP1);			// T1 = Z1^2
	BIGNUM_SUB_64_RE			(INX1,		TEMP1,		TEMP2);			// T2 = X1 - Z1^2
	BIGNUM_ADD_64_RE			(INX1,		TEMP1,		TEMP1);			// T1 = X1 + Z1^2
	BIGNUM_MUL_OS_64_RE			(TEMP2,		TEMP1,		TEMP2);			// T2 = X1^2 - Z1^4
	BIGNUM_MUL_OS_64_RE			(CONST3,	TEMP2,		TEMP2);			// T2 = A = 3 * (X1^2 - Z1^4)
	BIGNUM_MUL_OS_64_RE			(CONST2,	INY1,		OUTY);			// Y3 = B = 2 * Y1
	BIGNUM_MUL_OS_64_RE			(OUTY,		INZ1,		OUTZ);			// Z3 = B * Z1

	BIGNUM_SQU_OS_64_RE			(OUTY,					OUTY);			// Y3 = C = B^2
	BIGNUM_MUL_OS_64_RE			(OUTY,		INX1,		TEMP3);			// T3 = D = C * X1
	BIGNUM_SQU_OS_64_RE			(OUTY,					OUTY);			// Y3 = C^2
	BIGNUM_MUL_OS_64_RE			(OUTY,		P_256_D2,	OUTY);			// Y3 = C^2 / 2
	BIGNUM_SQU_OS_64_RE			(TEMP2,					OUTX);			// X3 = A^2
	BIGNUM_MUL_OS_64_RE			(CONST2,	TEMP3,		TEMP1);			// T1 = 2 * D
	BIGNUM_SUB_64_RE			(OUTX,		TEMP1,		OUTX);			// X3 = A^2 - 2 * D

	BIGNUM_SUB_64_RE			(TEMP3,		OUTX,		TEMP1);			// T1 = D - X3
	BIGNUM_MUL_OS_64_RE			(TEMP1,		TEMP2,		TEMP1);			// T1 = (D - X3) * A
	BIGNUM_SUB_64_RE			(TEMP1,		OUTY,		OUTY);			// Y3 = (D - X3) * A - C^2 / 2
	
EXIT:
	memcpy(output->x, OUTX, _WORD_32_ * sizeof(uint_32));
	memcpy(output->y, OUTY, _WORD_32_ * sizeof(uint_32));
	memcpy(output->z, OUTZ, _WORD_32_ * sizeof(uint_32));
}

void ECADD_Jacobian							(ECJ* input1,		ECJ* input2,				ECJ* output)
{
	uint_32 TEMP1[_WORD_32_]	= { 0, };
	uint_32 TEMP2[_WORD_32_]	= { 0, };
	uint_32 TEMP3[_WORD_32_]	= { 0, };
	uint_32 TEMP4[_WORD_32_]	= { 0, };
	uint_32 INVERSE[_WORD_32_]	= { 0, };
	uint_32 LAMBDA[_WORD_32_]	= { 0, };

	uint_32 INX1[_WORD_32_] = { 0, }, INY1[_WORD_32_] = { 0, }, INZ1[_WORD_32_] = { 0, };
	uint_32 INX2[_WORD_32_] = { 0, }, INY2[_WORD_32_] = { 0, }, INZ2[_WORD_32_] = { 0, };
	uint_32 OUTX[_WORD_32_] = { 0, }, OUTY[_WORD_32_] = { 0, }, OUTZ[_WORD_32_] = { 0, };

	uint_32 CONST2[_WORD_32_] = { 0, }; CONST2[0] = 0X00000002;
	
	ECJ TEMP;

	memcpy(INX1, input1->x, _WORD_32_ * sizeof(uint_32));
	memcpy(INY1, input1->y, _WORD_32_ * sizeof(uint_32));
	memcpy(INZ1, input1->z, _WORD_32_ * sizeof(uint_32));

	memcpy(INX2, input2->x, _WORD_32_ * sizeof(uint_32));
	memcpy(INY2, input2->y, _WORD_32_ * sizeof(uint_32));
	memcpy(INZ2, input2->z, _WORD_32_ * sizeof(uint_32));

	if (input2->flag == 1)
	{
		memcpy(OUTX, input1->x, _WORD_32_ * sizeof(uint_32));
		memcpy(OUTY, input1->y, _WORD_32_ * sizeof(uint_32));
		memcpy(OUTZ, input1->z, _WORD_32_ * sizeof(uint_32));
		goto EXIT;
	}

	if (input1->flag == 1)
	{
		memcpy(OUTX, input2->x, _WORD_32_ * sizeof(uint_32));
		memcpy(OUTY, input2->y, _WORD_32_ * sizeof(uint_32));
		memcpy(OUTZ, input2->z, _WORD_32_ * sizeof(uint_32));
		goto EXIT;
	}

	BIGNUM_SQU_OS_64_RE			(INZ1,					TEMP1);			// T1 = A = Z1^2
	BIGNUM_MUL_OS_64_RE			(TEMP1,		INZ1,		TEMP2);			// T2 = B = Z1 * A
	BIGNUM_MUL_OS_64_RE			(TEMP1,		INX2,		TEMP1);			// T1 = C = X2 * A

	BIGNUM_MUL_OS_64_RE			(TEMP2,		INY2,		TEMP2);			// T2 = D = Y2 * B
	BIGNUM_SUB_64_RE			(TEMP1,		INX1,		TEMP1);			// T1 = E = C - X1
	BIGNUM_SUB_64_RE			(TEMP2,		INY1,		TEMP2);			// T2 = F = D - Y1

	if (Check_Num0(TEMP1) == 1)
	{
		if (Check_Num0(TEMP2) == 1)
		{
			ECDBL_Jacobian(input2, &TEMP);
			memcpy(OUTX, TEMP.x, _WORD_32_ * sizeof(uint_32));
			memcpy(OUTY, TEMP.y, _WORD_32_ * sizeof(uint_32));
			memcpy(OUTZ, TEMP.z, _WORD_32_ * sizeof(uint_32));
			goto EXIT;
		}
		else
		{
			memcpy(output->flag, _INFINITY_, sizeof(char));
			goto EXIT;
		}
	}
	BIGNUM_MUL_OS_64_RE			(INZ1,		TEMP1,		OUTZ);			// Z3 = Z1 * E

	BIGNUM_SQU_OS_64_RE			(TEMP1,					TEMP3);			// T3 = G = E^2
	BIGNUM_MUL_OS_64_RE			(TEMP3,		TEMP1,		TEMP4);			// T4 = H = E^3
	BIGNUM_MUL_OS_64_RE			(TEMP3,		INX1,		TEMP3);			// T3 = I = X1 * G
	BIGNUM_MUL_OS_64_RE			(CONST2,	TEMP3,		TEMP1);			// T1 = 2 * I
	BIGNUM_SQU_OS_64_RE			(TEMP2,					OUTX);			// X3 = F^2

	BIGNUM_SUB_64_RE			(OUTX,		TEMP1,		OUTX);			// X3 = F^2 - 2 * I 
	BIGNUM_SUB_64_RE			(OUTX,		TEMP4,		OUTX);			// X3 = F^2 - (H + 2 * I)
	BIGNUM_SUB_64_RE			(TEMP3,		OUTX,		TEMP3);			// T3 = T3 - X3
	BIGNUM_MUL_OS_64_RE			(TEMP3,		TEMP2,		TEMP3);			// T3 = F * (T3 - X3)
	BIGNUM_MUL_OS_64_RE			(TEMP4,		INY1,		TEMP4);			// T4 = Y1 * H

	BIGNUM_SUB_64_RE			(TEMP3,		TEMP4,		OUTY);			// Y3 = F * (T3 - X3) - Y1 * H

EXIT:
	memcpy(output->x,		OUTX,	_WORD_32_ * sizeof(uint_32));
	memcpy(output->y,		OUTY, 	_WORD_32_ * sizeof(uint_32));
	memcpy(output->z,		OUTZ,	_WORD_32_ * sizeof(uint_32));
}

void Left_To_Right_Binary_MUL_Jacobian		(ECJ* input,		cuint32* inconst,			ECJ* output)
{
	ECJ Q; Q.flag = 1;
	ECJ P; P.flag = 0;
	uint_32 bit;
	uint_32 TEMP2[_WORD_32_] = { 0, };
	uint_32 TEMP3[_WORD_32_] = { 0, };
	uint_32 INVERSE[_WORD_32_] = { 0, };

	memcpy(P.x, input->x, _WORD_32_ * sizeof(uint_32));
	memcpy(P.y, input->y, _WORD_32_ * sizeof(uint_32));
	memcpy(P.z, input->z, _WORD_32_ * sizeof(uint_32));
	
	for (int cnt_i = _WORD_32_ - 1; cnt_i >= 0; cnt_i--)
	{
		for (int cnt_j = 31; cnt_j >= 0; cnt_j--)
		{
			bit = (inconst[cnt_i] >> cnt_j) & 0x1;

			if (Q.flag == 0)
				ECDBL_Jacobian(&Q, &Q);

			if (bit == 1) 
			{
				if (Q.flag == 1)
				{
					memcpy(Q.x, P.x, _WORD_32_ * sizeof(uint_32));
					memcpy(Q.y, P.y, _WORD_32_ * sizeof(uint_32));
					memcpy(Q.z, P.z, _WORD_32_ * sizeof(uint_32));
					Q.flag = 0;
				}
				else
					ECADD_Jacobian(&Q, &P, &Q);
			}
		}
	}

	BIGNUM_SQU_OS_64_RE(Q.z, TEMP2);
	BIGNUM_MUL_OS_64_RE(Q.z, TEMP2, TEMP3);

	Binary_Alg_Inversion_Alg(TEMP3, INVERSE);
	BIGNUM_MUL_OS_64_RE(Q.y, INVERSE, Q.y);

	BIGNUM_MUL_OS_64_RE(Q.z, INVERSE, INVERSE);
	BIGNUM_MUL_OS_64_RE(Q.x, INVERSE, Q.x);

	BIGNUM_MUL_OS_64_RE(Q.z, INVERSE, INVERSE);
	BIGNUM_MUL_OS_64_RE(Q.z, INVERSE, Q.z);

	memcpy(output->x, Q.x, _WORD_32_ * sizeof(uint_32));
	memcpy(output->y, Q.y, _WORD_32_ * sizeof(uint_32));
	memcpy(output->z, Q.z, _WORD_32_ * sizeof(uint_32));
}

void ECADD_Jacobian_INPUT3					(cuint32* inX,	cuint32* inY,	cuint32* inZ,	ECJ* input2,	ECJ* output)

{
	uint_32 TEMP1[_WORD_32_]	= { 0, };
	uint_32 TEMP2[_WORD_32_]	= { 0, };
	uint_32 TEMP3[_WORD_32_]	= { 0, };
	uint_32 TEMP4[_WORD_32_]	= { 0, };
	uint_32 INVERSE[_WORD_32_]	= { 0, };
	uint_32 LAMBDA[_WORD_32_]	= { 0, };

	uint_32 INX1[_WORD_32_] = { 0, }, INY1[_WORD_32_] = { 0, }, INZ1[_WORD_32_] = { 0, };
	uint_32 INX2[_WORD_32_] = { 0, }, INY2[_WORD_32_] = { 0, }, INZ2[_WORD_32_] = { 0, };
	uint_32 OUTX[_WORD_32_] = { 0, }, OUTY[_WORD_32_] = { 0, }, OUTZ[_WORD_32_] = { 0, };

	uint_32 CONST2[_WORD_32_] = { 0, }; CONST2[0] = 0X00000002;
	
	ECJ TEMP;

	memcpy(INX1, inX, _WORD_32_ * sizeof(uint_32));
	memcpy(INY1, inY, _WORD_32_ * sizeof(uint_32));
	memcpy(INZ1, inZ, _WORD_32_ * sizeof(uint_32));

	memcpy(INX2, input2->x, _WORD_32_ * sizeof(uint_32));
	memcpy(INY2, input2->y, _WORD_32_ * sizeof(uint_32));
	memcpy(INZ2, input2->z, _WORD_32_ * sizeof(uint_32));

	if (input2->flag == 1)
	{
		memcpy(OUTX, inX, _WORD_32_ * sizeof(uint_32));
		memcpy(OUTY, inY, _WORD_32_ * sizeof(uint_32));
		memcpy(OUTZ, inZ, _WORD_32_ * sizeof(uint_32));
		goto EXIT;
	}

	BIGNUM_SQU_OS_64_RE			(INZ1,					TEMP1);			// T1 = A = Z1^2
	BIGNUM_MUL_OS_64_RE			(TEMP1,		INZ1,		TEMP2);			// T2 = B = Z1 * A
	BIGNUM_MUL_OS_64_RE			(TEMP1,		INX2,		TEMP1);			// T1 = C = X2 * A

	BIGNUM_MUL_OS_64_RE			(TEMP2,		INY2,		TEMP2);			// T2 = D = Y2 * B
	BIGNUM_SUB_64_RE			(TEMP1,		INX1,		TEMP1);			// T1 = E = C - X1
	BIGNUM_SUB_64_RE			(TEMP2,		INY1,		TEMP2);			// T2 = F = D - Y1

	if (Check_Num0(TEMP1) == 1)
	{
		if (Check_Num0(TEMP2) == 1)
		{
			ECDBL_Jacobian(input2, &TEMP);
			memcpy(OUTX, TEMP.x, _WORD_32_ * sizeof(uint_32));
			memcpy(OUTY, TEMP.y, _WORD_32_ * sizeof(uint_32));
			memcpy(OUTZ, TEMP.z, _WORD_32_ * sizeof(uint_32));
			goto EXIT;
		}
		else
		{
			memcpy(output->flag, _INFINITY_, sizeof(char));
			goto EXIT;
		}
	}
	BIGNUM_MUL_OS_64_RE			(INZ1,		TEMP1,		OUTZ);			// Z3 = Z1 * E

	BIGNUM_SQU_OS_64_RE			(TEMP1,					TEMP3);			// T3 = G = E^2
	BIGNUM_MUL_OS_64_RE			(TEMP3,		TEMP1,		TEMP4);			// T4 = H = E^3
	BIGNUM_MUL_OS_64_RE			(TEMP3,		INX1,		TEMP3);			// T3 = I = X1 * G
	BIGNUM_MUL_OS_64_RE			(CONST2,	TEMP3,		TEMP1);			// T1 = 2 * I
	BIGNUM_SQU_OS_64_RE			(TEMP2,					OUTX);			// X3 = F^2

	BIGNUM_SUB_64_RE			(OUTX,		TEMP1,		OUTX);			// X3 = F^2 - 2 * I 
	BIGNUM_SUB_64_RE			(OUTX,		TEMP4,		OUTX);			// X3 = F^2 - (H + 2 * I)
	BIGNUM_SUB_64_RE			(TEMP3,		OUTX,		TEMP3);			// T3 = T3 - X3
	BIGNUM_MUL_OS_64_RE			(TEMP3,		TEMP2,		TEMP3);			// T3 = F * (T3 - X3)
	BIGNUM_MUL_OS_64_RE			(TEMP4,		INY1,		TEMP4);			// T4 = Y1 * H

	BIGNUM_SUB_64_RE			(TEMP3,		TEMP4,		OUTY);			// Y3 = F * (T3 - X3) - Y1 * H

EXIT:
	memcpy(output->x,		OUTX,	_WORD_32_ * sizeof(uint_32));
	memcpy(output->y,		OUTY, 	_WORD_32_ * sizeof(uint_32));
	memcpy(output->z,		OUTZ,	_WORD_32_ * sizeof(uint_32));
}