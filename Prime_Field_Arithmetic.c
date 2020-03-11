#include "Basic_Definitions.h"
#include "Prime_Field_Arithmetic.h"

uint_32  INVERSE_2[8]	= { 0X00000000, 0X00000000, 0X80000000, 0X00000000, 0X00000000, 0X80000000, 0X80000000, 0X7FFFFFFF };

int  BIGNUM_ADD_64						(cuint32* input1,		cuint32* input2,			uint_32* output)
{
	uint_64 temp_64 = 0;
	uint_32 carry = 0;

	for (int cnt_i = 0; cnt_i < _WORD_32_; cnt_i++)
	{
		temp_64			= (uint_64)input1[cnt_i] + input2[cnt_i] + carry;
		carry			= temp_64 >> 32;
		output[cnt_i]	= (uint_32)(temp_64 & 0x00000000FFFFFFFF);
	}

	return carry;
}

int  BIGNUM_SUB_64						(cuint32* input1,		cuint32* input2,			uint_32* output)
{
	uint_64 temp_64 = 0;
	uint_32 carry = 0;

	for (int cnt_i = 0; cnt_i < _WORD_32_; cnt_i++)
	{
		temp_64			= (uint_64)input1[cnt_i] - input2[cnt_i] - carry;
		carry			= (temp_64 >> 32) & 1;
		output[cnt_i]	= (uint_32)(temp_64 & 0x00000000FFFFFFFF);
	}

	return carry;
}

void BIGNUM_ADD_64_RE					(cuint32* input1,		cuint32* input2,			uint_32* output)
{
	uint_64 temp_64 = 0;
	uint_32 carry = 0;

	for (int cnt_i = 0; cnt_i < _WORD_32_; cnt_i++)
	{
		temp_64			= (uint_64)input1[cnt_i] + input2[cnt_i] + carry;
		carry			= temp_64 >> 32;
		output[cnt_i]	= (uint_32)(temp_64 & 0x00000000FFFFFFFF);
	}

	if (carry == 1)
		BIGNUM_ADD_64_RE(output, P_256_2C, output);
}

void BIGNUM_SUB_64_RE					(cuint32* input1,		cuint32* input2,			uint_32* output)
{
	uint_64 temp = 0;
	uint_32 carry = 0;

	for (int cnt_i = 0; cnt_i < _WORD_32_; cnt_i++)
	{
		temp			= (uint_64)input1[cnt_i] - input2[cnt_i] - carry;
		carry			= (temp >> 32) & 1;
		output[cnt_i]	= (uint_64)(temp & 0x00000000FFFFFFFF);
	}

	if (carry == 1)
		BIGNUM_SUB_64(output, P_256_2C, output);
}

void Reduction_64						(cuint32* Linput,									uint_32* output)
{
	uint_64 temp_64_P = 0,	temp_64_N = 0,	temp_64 = 0;
	uint_32 carry_P = 0,	carry_N = 0,	carry = 0;

	//	[0]

	temp_64_P	= (uint_64)Linput[ 9] + Linput[ 8] + Linput[ 0];
	temp_64_N	= (uint_64)Linput[14] + Linput[13] + Linput[12] + Linput[11];

	temp_64		= (uint_64)(temp_64_P & 0x00000000FFFFFFFF) - (temp_64_N & 0x00000000FFFFFFFF);
	carry		= (temp_64 >> 32) & 1;
	output[0]	= (uint_32)(temp_64 & 0x00000000FFFFFFFF);

	//	[1]

	temp_64_P	= (uint_64)(temp_64_P >> 32) + Linput[10] + Linput[ 9] + Linput[ 1];
	temp_64_N	= (uint_64)(temp_64_N >> 32) + Linput[15] + Linput[14] + Linput[13] + Linput[12];

	temp_64		= (uint_64)(temp_64_P & 0x00000000FFFFFFFF) - (temp_64_N & 0x00000000FFFFFFFF) - carry;
	carry		= (temp_64 >> 32) & 1;
	output[1]	= (uint_32)(temp_64 & 0x00000000FFFFFFFF);

	//	[2]
	
	temp_64_P	= (uint_64)(temp_64_P >> 32) + Linput[11] + Linput[10] + Linput[ 2];
	temp_64_N	= (uint_64)(temp_64_N >> 32) + Linput[15] + Linput[14] + Linput[13];

	temp_64		= (uint_64)(temp_64_P & 0x00000000FFFFFFFF) - (temp_64_N & 0x00000000FFFFFFFF) - carry;
	carry		= (temp_64 >> 32) & 1;
	output[2]	= (uint_32)(temp_64 & 0x00000000FFFFFFFF);
	
	//	[3]
		
	temp_64_P	= (uint_64)(temp_64_P >> 32) + Linput[13] + Linput[12] + Linput[12] + Linput[11] + Linput[11] + Linput[ 3];
	temp_64_N	= (uint_64)(temp_64_N >> 32) + Linput[15] + Linput[ 9] + Linput[ 8];

	temp_64		= (uint_64)(temp_64_P & 0x00000000FFFFFFFF) - (temp_64_N & 0x00000000FFFFFFFF) - carry;
	carry		= (temp_64 >> 32) & 1;
	output[3]	= (uint_32)(temp_64 & 0x00000000FFFFFFFF);
	
	//	[4]

	temp_64_P	= (uint_64)(temp_64_P >> 32) + Linput[14] + Linput[13] + Linput[13] + Linput[12] + Linput[12] + Linput[ 4];
	temp_64_N	= (uint_64)(temp_64_N >> 32) + Linput[10] + Linput[ 9];

	temp_64		= (uint_64)(temp_64_P & 0x00000000FFFFFFFF) - (temp_64_N & 0x00000000FFFFFFFF) - carry;
	carry		= (temp_64 >> 32) & 1;
	output[4]	= (uint_32)(temp_64 & 0x00000000FFFFFFFF);
	
	//	[5]

	temp_64_P	= (uint_64)(temp_64_P >> 32) + Linput[15] + Linput[14] + Linput[14] + Linput[13] + Linput[13] + Linput[ 5];
	temp_64_N	= (uint_64)(temp_64_N >> 32) + Linput[11] + Linput[10];

	temp_64		= (uint_64)(temp_64_P & 0x00000000FFFFFFFF) - (temp_64_N & 0x00000000FFFFFFFF) - carry;
	carry		= (temp_64 >> 32) & 1;
	output[5]	= (uint_32)(temp_64 & 0x00000000FFFFFFFF);
	
	//	[6]
	
	temp_64_P	= (uint_64)(temp_64_P >> 32) + Linput[15] + Linput[15] + Linput[14] + Linput[14] + Linput[14] + Linput[13] + Linput[ 6];
	temp_64_N	= (uint_64)(temp_64_N >> 32) + Linput[ 9] + Linput[ 8];

	temp_64		= (uint_64)(temp_64_P & 0x00000000FFFFFFFF) - (temp_64_N & 0x00000000FFFFFFFF) - carry;
	carry		= (temp_64 >> 32) & 1;
	output[6]	= (uint_32)(temp_64 & 0x00000000FFFFFFFF);
	
	//	[7]

	temp_64_P	= (uint_64)(temp_64_P >> 32) + Linput[15] + Linput[15] + Linput[15] + Linput[ 8] + Linput[ 7];
	temp_64_N	= (uint_64)(temp_64_N >> 32) + Linput[13] + Linput[12] + Linput[11] + Linput[10];

	temp_64		= (uint_64)(temp_64_P & 0x00000000FFFFFFFF) - (temp_64_N & 0x00000000FFFFFFFF) - carry;
	carry		= (temp_64 >> 32) & 1;
	output[7]	= (uint_32)(temp_64 & 0x00000000FFFFFFFF);
	
	carry_P = temp_64_P >> 32;
	carry_N = (temp_64_N >> 32) + carry;

	if (carry_P >= carry_N)
	{
		carry = carry_P - carry_N;
		while (carry > 0) 
		{
			BIGNUM_ADD_64(output, P_256_2C, output);
			carry--;
		}
	}
	if (carry_P < carry_N)
	{
		carry = carry_N - carry_P;
		while (carry > 0)
		{
			BIGNUM_SUB_64(output, P_256_2C, output);
			carry--;
		}
	}
}

void Fast_Reduction_64					(cuint32* Linput,									uint_32* output)
{
	uint_32 S1[_WORD_32_] = {	Linput[ 0],	Linput[ 1],	Linput[ 2],	Linput[ 3],	Linput[ 4],	Linput[ 5],	Linput[ 6],	Linput[ 7]	};
	uint_32 S2[_WORD_32_] = {	0,			0,			0,			Linput[11],	Linput[12],	Linput[13],	Linput[14],	Linput[15]	};
	uint_32 S3[_WORD_32_] = {	0,			0,			0,			Linput[12],	Linput[13],	Linput[14],	Linput[15],	0			};
	uint_32 S4[_WORD_32_] = {	Linput[ 8],	Linput[ 9],	Linput[10],	0,			0,			0,			Linput[14],	Linput[15]	};
	uint_32 S5[_WORD_32_] = {	Linput[ 9],	Linput[10],	Linput[11],	Linput[13],	Linput[14],	Linput[15],	Linput[13],	Linput[ 8]	};
	uint_32 S6[_WORD_32_] = {	Linput[11],	Linput[12], Linput[13],	0,			0,			0,			Linput[ 8],	Linput[10]	};
	uint_32 S7[_WORD_32_] = {	Linput[12],	Linput[13], Linput[14],	Linput[15],	0,			0,			Linput[ 9],	Linput[11]	};
	uint_32 S8[_WORD_32_] = {	Linput[13],	Linput[14],	Linput[15],	Linput[ 8],	Linput[ 9],	Linput[10],	0,			Linput[12]	};
	uint_32 S9[_WORD_32_] = {	Linput[14],	Linput[15],	0,			Linput[ 9],	Linput[10],	Linput[11],	0,			Linput[13],	};
	uint_32 TEMP[_WORD_32_] = { 0, };

	BIGNUM_ADD_64_RE(S1,		S2,		TEMP);
	BIGNUM_ADD_64_RE(TEMP,		S2,		TEMP);
	BIGNUM_ADD_64_RE(TEMP,		S3,		TEMP);
	BIGNUM_ADD_64_RE(TEMP,		S3,		TEMP);
	BIGNUM_ADD_64_RE(TEMP,		S4,		TEMP);
	BIGNUM_ADD_64_RE(TEMP,		S5,		TEMP);
	BIGNUM_SUB_64_RE(TEMP,		S6,		TEMP);
	BIGNUM_SUB_64_RE(TEMP,		S7,		TEMP);
	BIGNUM_SUB_64_RE(TEMP,		S8,		TEMP);
	BIGNUM_SUB_64_RE(TEMP,		S9,		TEMP);

	memcpy(output, TEMP, 8 * sizeof(uint_32));
}

void BIGNUM_MUL_OS_64					(cuint32* input1,		cuint32* input2,			uint_32* Loutput)
{
	uint_64 UV = 0;
	uint_32 TEMP[16] = { 0, };

	for (int cnt_i = 0; cnt_i < _WORD_32_; cnt_i++)
	{
		UV = UV & 0X00000000FFFFFFFF;
		for (int cnt_j = 0; cnt_j < _WORD_32_; cnt_j++)
		{
			UV = (uint_64)input1[cnt_i] * input2[cnt_j] + TEMP[cnt_i + cnt_j] + (UV >> 32);
			TEMP[cnt_i + cnt_j] = (uint_32)UV;
		}
		TEMP[cnt_i + _WORD_32_] = (UV >> 32);
	}
	memcpy(Loutput, TEMP, 16 * sizeof(uint_32));
}

void BIGNUM_SQU_OS_64					(cuint32* input,									uint_32* Loutput)
{
	uint_64 UV = 0;
	uint_32 TEMP[16] = { 0, };

	for (int cnt_i = 0; cnt_i < _WORD_32_; cnt_i++)
	{
		UV = UV & 0X00000000FFFFFFFF;
		for (int cnt_j = 0; cnt_j < _WORD_32_; cnt_j++)
		{
			UV = (uint_64)input[cnt_i] * input[cnt_j] + TEMP[cnt_i + cnt_j] + (UV >> 32);
			TEMP[cnt_i + cnt_j] = (uint_32)UV;
		}
		TEMP[cnt_i + _WORD_32_] = (UV >> 32);
	}
	memcpy(Loutput, TEMP, 16 * sizeof(uint_32));
}

void BIGNUM_MUL_OS_64_RE				(cuint32* input1,		cuint32* input2,			uint_32* output)
{
	uint_64 UV = 0;
	uint_32 TEMP[16] = { 0, };

	for (int cnt_i = 0; cnt_i < _WORD_32_; cnt_i++)
	{
		UV = UV & 0X00000000FFFFFFFF;
		for (int cnt_j = 0; cnt_j < _WORD_32_; cnt_j++)
		{
			UV = (uint_64)input1[cnt_i] * input2[cnt_j] + TEMP[cnt_i + cnt_j] + (UV >> 32);
			TEMP[cnt_i + cnt_j] = (uint_32)UV;
		}
		TEMP[cnt_i + _WORD_32_] = (UV >> 32);
	}
	Reduction_64(TEMP, output);
}

void BIGNUM_SQU_OS_64_RE				(cuint32* input,									uint_32* output)
{
	uint_64 UV = 0;
	uint_32 TEMP[16] = { 0, };

	for (int cnt_i = 0; cnt_i < _WORD_32_; cnt_i++)
	{
		UV = UV & 0X00000000FFFFFFFF;
		for (int cnt_j = 0; cnt_j < _WORD_32_; cnt_j++)
		{
			UV = (uint_64)input[cnt_i] * input[cnt_j] + TEMP[cnt_i + cnt_j] + (UV >> 32);
			TEMP[cnt_i + cnt_j] = (uint_32)UV;
		}
		TEMP[cnt_i + _WORD_32_] = (UV >> 32);
	}
	Reduction_64(TEMP, output);
}

void Fermat_Based_Inversion				(cuint32* input,									uint_32* output)
{
	uint_32 Z1[8] = { 0, };	// input
	uint_32 Z3[8] = { 0, };
	uint_32 ZF[8] = { 0, };
	uint_32 T0[8] = { 0, };
	uint_32 T1[8] = { 0, };
	uint_32 T2[8] = { 0, };
	uint_32 T3[8] = { 0, };
	uint_32 T4[8] = { 0, };
	uint_32 T5[8] = { 0, };
	uint_32 TEMP[8] = { 0, };

	memcpy(Z1, input, 8 * sizeof(uint_32));

	// Z3
	BIGNUM_SQU_OS_64_RE(Z1,				TEMP);			// Z^0000 0010
	BIGNUM_MUL_OS_64_RE(TEMP,	Z1,		Z3);			// Z^0000 0011

	// ZF
	BIGNUM_SQU_OS_64_RE(Z3,				TEMP);			// Z^0000 0110
	BIGNUM_SQU_OS_64_RE(TEMP,			TEMP);			// Z^0000 1100
	BIGNUM_MUL_OS_64_RE(TEMP,	Z3,		ZF);			// Z^0000 1111

	// T0
	BIGNUM_SQU_OS_64_RE(ZF,				TEMP);			// Z^0001 1110
	BIGNUM_SQU_OS_64_RE(TEMP,			TEMP);			// Z^0011 1100
	BIGNUM_MUL_OS_64_RE(TEMP,	Z3,		T0);			// Z^0011 1111

	// T1
	BIGNUM_SQU_OS_64_RE(T0,				TEMP);
	BIGNUM_SQU_OS_64_RE(TEMP,			TEMP);
	BIGNUM_SQU_OS_64_RE(TEMP,			TEMP);
	BIGNUM_SQU_OS_64_RE(TEMP,			TEMP);
	BIGNUM_SQU_OS_64_RE(TEMP,			TEMP);
	BIGNUM_SQU_OS_64_RE(TEMP,			TEMP);
	BIGNUM_MUL_OS_64_RE(TEMP,	T0,		T1);			// Z^0000 0FFF

	// T2
	BIGNUM_SQU_OS_64_RE(T1,				TEMP);
	for (int cnt_i = 0; cnt_i < 11; cnt_i++)
		BIGNUM_SQU_OS_64_RE(TEMP,			TEMP);
	BIGNUM_MUL_OS_64_RE(TEMP,	T1,		TEMP);			// Z^00FF FFFF
	for (int cnt_i = 0; cnt_i < 6; cnt_i++)
		BIGNUM_SQU_OS_64_RE(TEMP,			TEMP);
	BIGNUM_MUL_OS_64_RE(TEMP,	T0,		T2);			// Z^3FFF FFFF

	// T3
	BIGNUM_SQU_OS_64_RE(T2,				TEMP);
	BIGNUM_SQU_OS_64_RE(TEMP,			TEMP);			// Z^FFFF FFFC
	BIGNUM_MUL_OS_64_RE(TEMP,	Z3,		T3);			// Z^FFFF FFFF

	// T4
	BIGNUM_SQU_OS_64_RE(T3,				TEMP);
	for (int cnt_i = 0; cnt_i < 31; cnt_i++)
		BIGNUM_SQU_OS_64_RE(TEMP,			TEMP);
	BIGNUM_MUL_OS_64_RE(TEMP,	Z1,		TEMP);			// Z^FFFFFFFF 00000001
	for (int cnt_i = 0; cnt_i < 95; cnt_i++)
		BIGNUM_SQU_OS_64_RE(TEMP,			TEMP);
	BIGNUM_SQU_OS_64_RE(TEMP,			T4);			// Z^FFFFFFFF 00000001 00000000 00000000 00000000

	// T5
	BIGNUM_SQU_OS_64_RE(T4,				TEMP);
	for (int cnt_i = 0; cnt_i < 31; cnt_i++)
		BIGNUM_SQU_OS_64_RE(TEMP,			TEMP);
	BIGNUM_MUL_OS_64_RE(TEMP,	T3,		TEMP);			// Z^FFFFFFFF 00000001 00000000 00000000 00000000 FFFFFFFF
	BIGNUM_SQU_OS_64_RE(TEMP,			TEMP);
	for (int cnt_i = 0; cnt_i < 31; cnt_i++)
		BIGNUM_SQU_OS_64_RE(TEMP,			TEMP);
	BIGNUM_MUL_OS_64_RE(TEMP,	T3,		T5);			// Z^FFFFFFFF 00000001 00000000 00000000 00000000 FFFFFFFF FFFFFFFF

	// T
	BIGNUM_SQU_OS_64_RE(T5,				TEMP);
	for (int cnt_i = 0; cnt_i < 29; cnt_i++)
		BIGNUM_SQU_OS_64_RE(TEMP,			TEMP);
	BIGNUM_MUL_OS_64_RE(TEMP,	T2,		TEMP);			// Z^3FFFFFFF C0000000 40000000 00000000 00000000 3FFFFFFF FFFFFFFF FFFFFFFF
	BIGNUM_SQU_OS_64_RE(TEMP,			TEMP);
	BIGNUM_SQU_OS_64_RE(TEMP,			TEMP);
	BIGNUM_MUL_OS_64_RE(TEMP,	Z1,		TEMP);			// Z^FFFFFFFF C0000001 00000000 00000000 00000000 FFFFFFFF FFFFFFFF FFFFFFFD

	memcpy(output, TEMP, 8 * sizeof(uint_32));
}

int  Check_Num1							(cuint32* input)
{
	for (int cnt_i = 1; cnt_i < _WORD_32_; cnt_i++)
		if (input[cnt_i] != 0)
			return _FAILURE_;

	if (input[0] == 1)
		return _SUCCESS_;

	return _FAILURE_;
}

int  Compare_Two_Numbers				(cuint32* input1,		cuint32* input2)
{
	for (int cnt_i = _WORD_32_ - 1; cnt_i >= 0; cnt_i--)
	{
		if (input1[cnt_i] > input2[cnt_i])
			return _SUCCESS_;
		if (input1[cnt_i] < input2[cnt_i])
			return _FAILURE_;
	}
	return _SUCCESS_;
}

void Binary_Alg_Inversion_Alg			(cuint32* input,									uint_32* output)
{
	
	uint_32 U[_WORD_32_] = { 0, };
	uint_32 V[_WORD_32_] = { 0, };
	uint_32 X1[_WORD_32_] = { 0, };
	uint_32 X2[_WORD_32_] = { 0, };
	uint_64 temp = 0;
	uint_32 carry = 0;

	memcpy(U, input, 8 * sizeof(uint_32));
	memcpy(V, P_256, 8 * sizeof(uint_32));

	X1[0] = 0X00000001;

	while ((Check_Num1(U) == _FAILURE_) && (Check_Num1(V) == _FAILURE_))
	{
		while ((U[0] & 0x1) == 0)
		{
			U[0] = (U[1] << 31) | (U[0] >> 1); U[1] = (U[2] << 31) | (U[1] >> 1);
			U[2] = (U[3] << 31) | (U[2] >> 1); U[3] = (U[4] << 31) | (U[3] >> 1);
			U[4] = (U[5] << 31) | (U[4] >> 1); U[5] = (U[6] << 31) | (U[5] >> 1);
			U[6] = (U[7] << 31) | (U[6] >> 1); U[7] = U[7] >> 1;

			if ((X1[0] & 0x1) == 0)
			{
				X1[0] = (X1[1] << 31) | (X1[0] >> 1); X1[1] = (X1[2] << 31) | (X1[1] >> 1);
				X1[2] = (X1[3] << 31) | (X1[2] >> 1); X1[3] = (X1[4] << 31) | (X1[3] >> 1);
				X1[4] = (X1[5] << 31) | (X1[4] >> 1); X1[5] = (X1[6] << 31) | (X1[5] >> 1);
				X1[6] = (X1[7] << 31) | (X1[6] >> 1); X1[7] = X1[7] >> 1;
			}
			else
			{
				X1[0] = (X1[1] << 31) | (X1[0] >> 1); X1[1] = (X1[2] << 31) | (X1[1] >> 1);
				X1[2] = (X1[3] << 31) | (X1[2] >> 1); X1[3] = (X1[4] << 31) | (X1[3] >> 1);
				X1[4] = (X1[5] << 31) | (X1[4] >> 1); X1[5] = (X1[6] << 31) | (X1[5] >> 1);
				X1[6] = (X1[7] << 31) | (X1[6] >> 1); X1[7] = X1[7] >> 1;

				carry = 0;
				for (int cnt_i = 0; cnt_i < _WORD_32_; cnt_i++)
				{
					temp		= (uint_64)X1[cnt_i] + P_256_D2[cnt_i] + carry;
					carry		= temp >> 32;
					X1[cnt_i]	= (uint_32)(temp & 0x00000000FFFFFFFF);
				}
			}
		}
		while ((V[0] & 0x1) == 0)
		{
			V[0] = (V[1] << 31) | (V[0] >> 1); V[1] = (V[2] << 31) | (V[1] >> 1);
			V[2] = (V[3] << 31) | (V[2] >> 1); V[3] = (V[4] << 31) | (V[3] >> 1);
			V[4] = (V[5] << 31) | (V[4] >> 1); V[5] = (V[6] << 31) | (V[5] >> 1);
			V[6] = (V[7] << 31) | (V[6] >> 1); V[7] = V[7] >> 1;

			if ((X2[0] & 0x1) == 0)
			{
				X2[0] = (X2[1] << 31) | (X2[0] >> 1); X2[1] = (X2[2] << 31) | (X2[1] >> 1);
				X2[2] = (X2[3] << 31) | (X2[2] >> 1); X2[3] = (X2[4] << 31) | (X2[3] >> 1);
				X2[4] = (X2[5] << 31) | (X2[4] >> 1); X2[5] = (X2[6] << 31) | (X2[5] >> 1);
				X2[6] = (X2[7] << 31) | (X2[6] >> 1); X2[7] = X2[7] >> 1;
			}
			else
			{
				X2[0] = (X2[1] << 31) | (X2[0] >> 1); X2[1] = (X2[2] << 31) | (X2[1] >> 1);
				X2[2] = (X2[3] << 31) | (X2[2] >> 1); X2[3] = (X2[4] << 31) | (X2[3] >> 1);
				X2[4] = (X2[5] << 31) | (X2[4] >> 1); X2[5] = (X2[6] << 31) | (X2[5] >> 1);
				X2[6] = (X2[7] << 31) | (X2[6] >> 1); X2[7] = X2[7] >> 1;
				carry = 0;
				for (int cnt_i = 0; cnt_i < _WORD_32_; cnt_i++)
				{
					temp = (uint_64)X2[cnt_i] + P_256_D2[cnt_i] + carry;
					carry = temp >> 32;
					X2[cnt_i] = (uint_32)(temp & 0x00000000FFFFFFFF);
				}
			}
		}
		if (Compare_Two_Numbers(U, V) == 1)
		{
			BIGNUM_SUB_64_RE(U, V, U);
			BIGNUM_SUB_64_RE(X1, X2, X1);
		}
		else
		{
			BIGNUM_SUB_64_RE(V, U, V);
			BIGNUM_SUB_64_RE(X2, X1, X2);
		}
	}

	if (Check_Num1(U) == _SUCCESS_)
	{
		if (Compare_Two_Numbers(X1, P_256) == 1)
		{
			BIGNUM_ADD_64(X1, P_256_2C, X1);
		}
		memcpy(output, &X1, 8 * sizeof(uint_32));
	}
	else
	{
		if (Compare_Two_Numbers(X2, P_256) == 1)
		{
			BIGNUM_ADD_64(X2, P_256_2C, X2);
		}
		memcpy(output, X2, 8 * sizeof(uint_32));
	}
}