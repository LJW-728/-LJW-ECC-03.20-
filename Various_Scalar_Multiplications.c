#include "Basic_Definitions.h"
#include "Prime_Field_Arithmetic.h"
#include "Elliptic_Curve_Operations_using_Jacobian_Coordinate.h"
#include "Various_Scalar_Multiplications.h"

uint_32 PsX[256][_WORD_32_] = { 0, };
uint_32 PsY[256][_WORD_32_] = { 0, };
uint_32 PsZ[256][_WORD_32_] = { 0, };

void Width_w_NAF_of_Positive_Integer		(cuint32* input,	int width_w,									uchar_8* output)
{
	uint_32 TEMP[_WORD_32_] = { 0, };
	TEMP[0] = 0X00000010;
	uint_32 K[_WORD_32_] = { 0, };
	uchar_8 Ki[257] = { 0, };
	int count = 0;
	uchar_8 WIDTH_W = 1;
	uchar_8 WIDTH_MAX = 1;
	uchar_8 WIDTH_CUT = 0;
	uchar_8 TEMP_8 = 0;
	
	WIDTH_W = WIDTH_W << (width_w - 1);
	WIDTH_MAX = (WIDTH_W << 1);
	WIDTH_CUT = (WIDTH_W << 1) - 1;

	memcpy(K, input, _WORD_32_ * sizeof(uint_32));

	while (Check_Num0(K) == _FAILURE_)
	{
		if ((K[0] & 0x1) == 1)
		{
			Ki[count] = K[0] & WIDTH_CUT;
			if (Ki[count] < WIDTH_W) {
				K[0] -= Ki[count];
			}
			else {
				TEMP_8 = WIDTH_MAX - Ki[count];
				Ki[count] = TEMP_8 | WIDTH_W;
				TEMP[0] = TEMP_8;
				BIGNUM_ADD_64_RE(K, TEMP, K);
			}
		}
		else
			Ki[count] = 0;
		K[0] = (K[1] << 31) | (K[0] >> 1); K[1] = (K[2] << 31) | (K[1] >> 1);
		K[2] = (K[3] << 31) | (K[2] >> 1); K[3] = (K[4] << 31) | (K[3] >> 1);
		K[4] = (K[5] << 31) | (K[4] >> 1); K[5] = (K[6] << 31) | (K[5] >> 1);
		K[6] = (K[7] << 31) | (K[6] >> 1); K[7] = K[7] >> 1;
		count++;
	}
	memcpy(output, Ki, 257 * sizeof(char));
}

void NAF_Method_for_Multiplication			(ECJ* input,		cuint32* inconst,		int width_w,			ECJ* output)
{
	ECJ Q; Q.flag = 1;
	ECJ P; P.flag = 0;
	uint_32 TEMP2[_WORD_32_] = { 0, };
	uint_32 TEMP3[_WORD_32_] = { 0, };
	uint_32 INVERSE[_WORD_32_] = { 0, };

	ECJ EC_TEMP; EC_TEMP.flag = 0;
	uint_32 PX[16][_WORD_32_] = { 0, };
	uint_32 PY[16][_WORD_32_] = { 0, };
	uint_32 PZ[16][_WORD_32_] = { 0, };
	uint_32 CONST[_WORD_32_] = { 0, };

	uchar_8 NAF[257] = { 0, };
	Width_w_NAF_of_Positive_Integer(inconst, width_w, NAF);

	int NUM = 0;
	int WIDTH = 1;
	WIDTH = 1 << (width_w - 1);

	memcpy(P.x, input->x, _WORD_32_ * sizeof(uint_32));
	memcpy(P.y, input->y, _WORD_32_ * sizeof(uint_32));
	memcpy(P.z, input->z, _WORD_32_ * sizeof(uint_32));

	CONST[0] = 1;
	for (int cnt_i = 0; cnt_i < WIDTH; cnt_i++)
	{
		Left_To_Right_Binary_MUL_Jacobian(&P, CONST, &EC_TEMP);
		memcpy(PX[cnt_i], EC_TEMP.x, _WORD_32_ * sizeof(uint_32));
		memcpy(PY[cnt_i], EC_TEMP.y, _WORD_32_ * sizeof(uint_32));
		memcpy(PZ[cnt_i], EC_TEMP.z, _WORD_32_ * sizeof(uint_32));
		CONST[0] = CONST[0] + 2;
	}

	WIDTH = WIDTH - 1;

	for (int cnt_i = 256; cnt_i >= 0; cnt_i--)
	{
		if (Q.flag == 0)
			ECDBL_Jacobian(&Q, &Q);

		if (NAF[cnt_i] != 0)
		{
			NUM = (NAF[cnt_i] & WIDTH) / 2;
			memcpy(P.x, PX[NUM], _WORD_32_ * sizeof(uint_32));
			memcpy(P.y, PY[NUM], _WORD_32_ * sizeof(uint_32));
			memcpy(P.z, PZ[NUM], _WORD_32_ * sizeof(uint_32));

			if (NAF[cnt_i] > WIDTH)
				BIGNUM_SUB_64_RE(P_256, P.y, P.y);
			
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

void Divide_String_Equal_Parts				(cuint32* input,													uchar_8* output)
{
	uchar_8 TEMP[32] = { 0, };
	uint_32 S0 = 0, S1 = 0, S2 = 0, S3 = 0, S4 = 0, S5 = 0, S6 = 0, S7 = 0;
	uint_32 C = 0x80000000;

	S7 = 24, S6 = 25, S5 = 26, S4 = 27, S3 = 28, S2 = 29, S1 = 30, S0 = 31;
	for (int cnt_i = 31; cnt_i > 7; cnt_i--)
	{
		TEMP[cnt_i] = ((input[7] & C) >> S7) | ((input[6] & C) >> S6) | ((input[5] & C) >> S5) | ((input[4] & C) >> S4) | ((input[3] & C) >> S3) | ((input[2] & C) >> S2) | ((input[1] & C) >> S1) | ((input[0] & C) >> S0);
		S7--, S6--, S5--, S4--, S3--, S2--, S1--, S0--;
		C = C >> 1;
	}
	TEMP[7] = ((input[7] & C) >> 0) | ((input[6] & C) >> 1) | ((input[5] & C) >> 2) | ((input[4] & C) >> 3) | ((input[3] & C) >> 4) | ((input[2] & C) >> 5) | ((input[1] & C) >> 6) | ((input[0] & C) >> 7);
	C = C >> 1;
	TEMP[6] = ((input[7] & C) << 1) | ((input[6] & C) >> 0) | ((input[5] & C) >> 1) | ((input[4] & C) >> 2) | ((input[3] & C) >> 3) | ((input[2] & C) >> 4) | ((input[1] & C) >> 5) | ((input[0] & C) >> 6);
	C = C >> 1;
	TEMP[5] = ((input[7] & C) << 2) | ((input[6] & C) << 1) | ((input[5] & C) >> 0) | ((input[4] & C) >> 1) | ((input[3] & C) >> 2) | ((input[2] & C) >> 3) | ((input[1] & C) >> 4) | ((input[0] & C) >> 5);
	C = C >> 1;
	TEMP[4] = ((input[7] & C) << 3) | ((input[6] & C) << 2) | ((input[5] & C) << 1) | ((input[4] & C) >> 0) | ((input[3] & C) >> 1) | ((input[2] & C) >> 2) | ((input[1] & C) >> 3) | ((input[0] & C) >> 4);
	C = C >> 1;
	TEMP[3] = ((input[7] & C) << 4) | ((input[6] & C) << 3) | ((input[5] & C) << 2) | ((input[5] & C) << 1) | ((input[3] & C) >> 0) | ((input[2] & C) >> 1) | ((input[1] & C) >> 2) | ((input[0] & C) >> 3);
	C = C >> 1;
	TEMP[2] = ((input[7] & C) << 5) | ((input[6] & C) << 4) | ((input[5] & C) << 3) | ((input[5] & C) << 2) | ((input[4] & C) >> 1) | ((input[2] & C) >> 0) | ((input[1] & C) >> 1) | ((input[0] & C) >> 2);
	C = C >> 1;
	TEMP[1] = ((input[7] & C) << 6) | ((input[6] & C) << 5) | ((input[5] & C) << 4) | ((input[5] & C) << 3) | ((input[4] & C) >> 2) | ((input[2] & C) << 1) | ((input[1] & C) >> 0) | ((input[0] & C) >> 1);
	C = C >> 1;
	TEMP[0] = ((input[7] & C) << 7) | ((input[6] & C) << 6) | ((input[5] & C) << 5) | ((input[4] & C) << 4) | ((input[3] & C) << 3) | ((input[2] & C) << 2) | ((input[1] & C) << 1) | ((input[0] & C) >> 0);
	
	printf("T : \t"); for (int cnt_i = 31; cnt_i >= 0; cnt_i--)printf("%X ", TEMP[cnt_i]); printf("\n");
	memcpy(output, TEMP, 32 * sizeof(char));
}

void Fixed_Base_Comb_Method					(ECJ* input,		cuint32* inconst,								ECJ* output)
{
	ECJ Q; Q.flag = 1;
	ECJ P; P.flag = 0;
	uint_32 TEMP2[_WORD_32_] = { 0, };
	uint_32 TEMP3[_WORD_32_] = { 0, };
	uint_32 INVERSE[_WORD_32_] = { 0, };

	ECJ EC_TEMP; EC_TEMP.flag = 0;
	ECJ EC_TEMP2; EC_TEMP2.flag = 0;
	uint_32 CONST[_WORD_32_] = { 0, }; CONST[0] = 1;
	
	memcpy(P.x, input->x, _WORD_32_ * sizeof(uint_32));
	memcpy(P.y, input->y, _WORD_32_ * sizeof(uint_32));
	memcpy(P.z, input->z, _WORD_32_ * sizeof(uint_32));

	uint_32 PX[8][_WORD_32_] = { 0, };
	uint_32 PY[8][_WORD_32_] = { 0, };
	uint_32 PZ[8][_WORD_32_] = { 0, };

	for (int cnt_i = 0; cnt_i < 7; cnt_i++)
	{
		Left_To_Right_Binary_MUL_Jacobian(&P, CONST, &EC_TEMP);
		memcpy(PX[cnt_i], EC_TEMP.x, _WORD_32_ * sizeof(uint_32));
		memcpy(PY[cnt_i], EC_TEMP.y, _WORD_32_ * sizeof(uint_32));
		memcpy(PZ[cnt_i], EC_TEMP.z, _WORD_32_ * sizeof(uint_32));
		for (int cnt_j = _WORD_32_ - 1; cnt_j >= 0; cnt_j--) printf("%08X", PX[cnt_i][cnt_j]); printf("\n\n");
		for (int cnt_j = _WORD_32_ - 1; cnt_j >= 0; cnt_j--) printf("%08X", PY[cnt_i][cnt_j]); printf("\n\n");
		CONST[cnt_i] = 0;
		CONST[cnt_i + 1] = 1;
	}
	Left_To_Right_Binary_MUL_Jacobian(&P, CONST, &EC_TEMP);
	memcpy(PX[7], EC_TEMP.x, _WORD_32_ * sizeof(uint_32));
	memcpy(PY[7], EC_TEMP.y, _WORD_32_ * sizeof(uint_32));
	memcpy(PZ[7], EC_TEMP.z, _WORD_32_ * sizeof(uint_32));

	CONST[7] = 1;
	Left_To_Right_Binary_MUL_Jacobian(&P, CONST, &EC_TEMP);
	for (int cnt_i = 128; cnt_i < 256; cnt_i++)
	{
		memcpy(PsX[cnt_i], EC_TEMP.x, _WORD_32_ * sizeof(uint_32));
		memcpy(PsY[cnt_i], EC_TEMP.y, _WORD_32_ * sizeof(uint_32));
		memcpy(PsZ[cnt_i], EC_TEMP.z, _WORD_32_ * sizeof(uint_32));
	}

	uchar_8 STRING[32] = { 0, };
	Divide_String_Equal_Parts(inconst, STRING);

	uchar_8 NUM = 1;
	int cnt_j = 0;

	for (cnt_j = 31; cnt_j >= 0; cnt_j--)
	{
		NUM = STRING[cnt_j];

		if (Q.flag == 0)
			ECDBL_Jacobian(&Q, &Q);

		printf("N : %8X\n", NUM);
		if (NUM != 0)
		{
			if (Q.flag == 1)
			{
				memcpy(Q.x, P.x, _WORD_32_ * sizeof(uint_32));
				memcpy(Q.y, P.y, _WORD_32_ * sizeof(uint_32));
				memcpy(Q.z, P.z, _WORD_32_ * sizeof(uint_32));
				Q.flag = 0;
			}
			else
			{

				if ((NUM & 0x1) == 1)
				{
					printf("1\n");
					memcpy(P.x, PX[7], _WORD_32_ * sizeof(uint_32));
					memcpy(P.y, PY[7], _WORD_32_ * sizeof(uint_32));
					memcpy(P.z, PZ[7], _WORD_32_ * sizeof(uint_32));
					ECADD_Jacobian(&Q, &P, &Q); Q.flag = 0;
					for (int cnt_i = _WORD_32_ - 1; cnt_i >= 0; cnt_i--)printf("%08X", P.x[cnt_i]); printf("\n\n");
					for (int cnt_i = _WORD_32_ - 1; cnt_i >= 0; cnt_i--)printf("%08X", P.y[cnt_i]); printf("\n\n");
				}
				if (((NUM >> 1) & 0x1) == 1)
				{
					printf("2\n");

					memcpy(P.x, PX[6], _WORD_32_ * sizeof(uint_32));
					memcpy(P.y, PY[6], _WORD_32_ * sizeof(uint_32));
					memcpy(P.z, PZ[6], _WORD_32_ * sizeof(uint_32));
					ECADD_Jacobian(&Q, &P, &Q); Q.flag = 0;
					for (int cnt_i = _WORD_32_ - 1; cnt_i >= 0; cnt_i--)printf("%08X", P.x[cnt_i]); printf("\n\n");
					for (int cnt_i = _WORD_32_ - 1; cnt_i >= 0; cnt_i--)printf("%08X", P.y[cnt_i]); printf("\n\n");
				}
				if (((NUM >> 2) & 0x1) == 1)
				{
					printf("4\n");

					memcpy(P.x, PX[5], _WORD_32_ * sizeof(uint_32));
					memcpy(P.y, PY[5], _WORD_32_ * sizeof(uint_32));
					memcpy(P.z, PZ[5], _WORD_32_ * sizeof(uint_32));
					ECADD_Jacobian(&Q, &P, &Q); Q.flag = 0;
					for (int cnt_i = _WORD_32_ - 1; cnt_i >= 0; cnt_i--)printf("%08X", P.x[cnt_i]); printf("\n\n");
					for (int cnt_i = _WORD_32_ - 1; cnt_i >= 0; cnt_i--)printf("%08X", P.y[cnt_i]); printf("\n\n");
				}
				if (((NUM >> 3) & 0x1) == 1)
				{
					printf("8\n");

					memcpy(P.x, PX[4], _WORD_32_ * sizeof(uint_32));
					memcpy(P.y, PY[4], _WORD_32_ * sizeof(uint_32));
					memcpy(P.z, PZ[4], _WORD_32_ * sizeof(uint_32));
					ECADD_Jacobian(&Q, &P, &Q); Q.flag = 0;
					for (int cnt_i = _WORD_32_ - 1; cnt_i >= 0; cnt_i--)printf("%08X", P.x[cnt_i]); printf("\n\n");
					for (int cnt_i = _WORD_32_ - 1; cnt_i >= 0; cnt_i--)printf("%08X", P.y[cnt_i]); printf("\n\n");
				}
				if (((NUM >> 4) & 0x1) == 1)
				{
					printf("16\n");

					memcpy(P.x, PX[3], _WORD_32_ * sizeof(uint_32));
					memcpy(P.y, PY[3], _WORD_32_ * sizeof(uint_32));
					memcpy(P.z, PZ[3], _WORD_32_ * sizeof(uint_32));
					ECADD_Jacobian(&Q, &P, &Q); Q.flag = 0;
					for (int cnt_i = _WORD_32_ - 1; cnt_i >= 0; cnt_i--)printf("%08X", P.x[cnt_i]); printf("\n\n");
					for (int cnt_i = _WORD_32_ - 1; cnt_i >= 0; cnt_i--)printf("%08X", P.y[cnt_i]); printf("\n\n");
				}
				if (((NUM >> 5) & 1) == 1)
				{
					printf("32\n");

					memcpy(P.x, PX[2], _WORD_32_ * sizeof(uint_32));
					memcpy(P.y, PY[2], _WORD_32_ * sizeof(uint_32));
					memcpy(P.z, PZ[2], _WORD_32_ * sizeof(uint_32));
					ECADD_Jacobian(&Q, &P, &Q); Q.flag = 0;
					for (int cnt_i = _WORD_32_ - 1; cnt_i >= 0; cnt_i--)printf("%08X", P.x[cnt_i]); printf("\n\n");
					for (int cnt_i = _WORD_32_ - 1; cnt_i >= 0; cnt_i--)printf("%08X", P.y[cnt_i]); printf("\n\n");
				}
				if (((NUM >> 6) & 1) == 1)
				{
					printf("64\n");

					memcpy(P.x, PX[1], _WORD_32_ * sizeof(uint_32));
					memcpy(P.y, PY[1], _WORD_32_ * sizeof(uint_32));
					memcpy(P.z, PZ[1], _WORD_32_ * sizeof(uint_32));
					ECADD_Jacobian(&Q, &P, &Q); Q.flag = 0;
					for (int cnt_i = _WORD_32_ - 1; cnt_i >= 0; cnt_i--)printf("%08X", P.x[cnt_i]); printf("\n\n");
					for (int cnt_i = _WORD_32_ - 1; cnt_i >= 0; cnt_i--)printf("%08X", P.y[cnt_i]); printf("\n\n");
				}
				if (((NUM >> 7) & 1) == 1)
				{
					printf("128\n");

					memcpy(P.x, PX[0], _WORD_32_ * sizeof(uint_32));
					memcpy(P.y, PY[0], _WORD_32_ * sizeof(uint_32));
					memcpy(P.z, PZ[0], _WORD_32_ * sizeof(uint_32));
					ECADD_Jacobian(&Q, &P, &Q); Q.flag = 0;
					for (int cnt_i = _WORD_32_ - 1; cnt_i >= 0; cnt_i--)printf("%08X", P.x[cnt_i]); printf("\n\n");
					for (int cnt_i = _WORD_32_ - 1; cnt_i >= 0; cnt_i--)printf("%08X", P.y[cnt_i]); printf("\n\n");
				}
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

void Fixed_Base_Comb_Method4					(ECJ* input,		cuint32* inconst,								ECJ* output)
{
	ECJ Q; Q.flag = 1;
	ECJ P; P.flag = 0;
	uint_32 TEMP2[_WORD_32_] = { 0, };
	uint_32 TEMP3[_WORD_32_] = { 0, };
	uint_32 INVERSE[_WORD_32_] = { 0, };

	ECJ EC_TEMP; EC_TEMP.flag = 0;
	ECJ EC_TEMP2; EC_TEMP2.flag = 0;
	uint_32 CONST[_WORD_32_] = { 0, };
	
	memcpy(P.x, input->x, _WORD_32_ * sizeof(uint_32));
	memcpy(P.y, input->y, _WORD_32_ * sizeof(uint_32));
	memcpy(P.z, input->z, _WORD_32_ * sizeof(uint_32));

	//uint_32 PX[8][_WORD_32_] = { 0, };
	//uint_32 PY[8][_WORD_32_] = { 0, };
	//uint_32 PZ[8][_WORD_32_] = { 0, };

	//for (int cnt_i = 0; cnt_i < 7; cnt_i++)
	//{
	//	Left_To_Right_Binary_MUL_Jacobian(&P, CONST, &EC_TEMP);
	//	memcpy(PsX[cnt_i], EC_TEMP.x, _WORD_32_ * sizeof(uint_32));
	//	memcpy(PsY[cnt_i], EC_TEMP.y, _WORD_32_ * sizeof(uint_32));
	//	memcpy(PsZ[cnt_i], EC_TEMP.z, _WORD_32_ * sizeof(uint_32));
	//	CONST[cnt_i] = 0;
	//	CONST[cnt_i + 1] = 1;
	//}
	//Left_To_Right_Binary_MUL_Jacobian(&P, CONST, &EC_TEMP);
	//memcpy(PsX[7], EC_TEMP.x, _WORD_32_ * sizeof(uint_32));
	//memcpy(PsY[7], EC_TEMP.y, _WORD_32_ * sizeof(uint_32));
	//memcpy(PsZ[7], EC_TEMP.z, _WORD_32_ * sizeof(uint_32));
	Left_To_Right_Binary_MUL_Jacobian(&P, CONST, &EC_TEMP);
	memcpy(PsX[0], EC_TEMP.x, _WORD_32_ * sizeof(uint_32));
	memcpy(PsY[0], EC_TEMP.y, _WORD_32_ * sizeof(uint_32));
	memcpy(PsZ[0], EC_TEMP.z, _WORD_32_ * sizeof(uint_32));

	CONST[7] = 1;
	Left_To_Right_Binary_MUL_Jacobian(&P, CONST, &EC_TEMP);
	for (int cnt_i = 128; cnt_i < 256; cnt_i++)
	{
		memcpy(PsX[cnt_i], EC_TEMP.x, _WORD_32_ * sizeof(uint_32));
		memcpy(PsY[cnt_i], EC_TEMP.y, _WORD_32_ * sizeof(uint_32));
		memcpy(PsZ[cnt_i], EC_TEMP.z, _WORD_32_ * sizeof(uint_32));
	}

	uint_32 count1 = 6;
	uint_32 count2 = 7;
	uint_32 CNT_K = 0;
	uint_32 CNT_J = 64;
	uint_32 CNT_I = 0;
	uint_32 CNT_P = 2;

	uint_32 CNT_X = 128;
	ECJ PT; PT.flag = 0;
	uint_32 START = 0;
	printf("START : %d\n", START);
	for (int cnt_k = 0; cnt_k < 7; cnt_k++)
	{
		printf("ROUND : %d \t| 0 : %d \t| 1 : %d \n", cnt_k, count2, count1);
		CONST[count1] = 1; count1--;
		CONST[count2] = 0; count2--;

		Left_To_Right_Binary_MUL_Jacobian(&P, CONST, &EC_TEMP);
		for (int cnt_j = CNT_J; cnt_j < 2 * CNT_J; cnt_j++)
		{
			printf("START1 : %d \t| %d\n", START, cnt_j); START++;
			memcpy(PsX[cnt_j], EC_TEMP.x, _WORD_32_ * sizeof(uint_32));
			memcpy(PsY[cnt_j], EC_TEMP.y, _WORD_32_ * sizeof(uint_32));
			memcpy(PsZ[cnt_j], EC_TEMP.z, _WORD_32_ * sizeof(uint_32));
		}
		printf("\n");

		CNT_K = CNT_P - 1;
		CNT_P = 2 * CNT_P;
		CNT_I = 3;
		while (CNT_K != 0)
		{
			for (int cnt_j = CNT_I * CNT_J; cnt_j < (CNT_I * CNT_J) + CNT_J; cnt_j++)
			{
				printf("START2 : %d \t| %d\n", START, cnt_j); START++;
				memcpy(EC_TEMP2.x, PsX[cnt_j], _WORD_32_ * sizeof(uint_32));
				memcpy(EC_TEMP2.y, PsY[cnt_j], _WORD_32_ * sizeof(uint_32));
				memcpy(EC_TEMP2.z, PsZ[cnt_j], _WORD_32_ * sizeof(uint_32));
				ECADD_Jacobian(&EC_TEMP2, &EC_TEMP, &EC_TEMP2);
				memcpy(PsX[cnt_j], EC_TEMP2.x, _WORD_32_ * sizeof(uint_32));
				memcpy(PsY[cnt_j], EC_TEMP2.y, _WORD_32_ * sizeof(uint_32));
				memcpy(PsZ[cnt_j], EC_TEMP2.z, _WORD_32_ * sizeof(uint_32));
			}
			printf("\n");
			printf("CNT_K: %d==================\n", CNT_K);
			CNT_K--;
			CNT_I = CNT_I + 2;
		}
		CNT_J = CNT_J/2;
	}

	uchar_8 STRING[32] = { 0, };
	Divide_String_Equal_Parts(inconst, STRING);

	uchar_8 NUM = 1;
	int cnt_j = 0;

	for (cnt_j = 31; cnt_j >= 0; cnt_j--)
	{
		NUM = STRING[cnt_j];

		if (Q.flag == 0)
		{
			printf("cnt_j : %d \t| NUM : %d \t| ECDBL_Jacobian\n", cnt_j, NUM);
			ECDBL_Jacobian(&Q, &Q);
		}

		if (NUM != 0)
		{
			if (Q.flag == 1)
			{
				printf("cnt_j : %d \t| NUM : %d \t| Q.flag == 1\n", cnt_j, NUM);
				memcpy(Q.x, P.x, _WORD_32_ * sizeof(uint_32));
				memcpy(Q.y, P.y, _WORD_32_ * sizeof(uint_32));
				memcpy(Q.z, P.z, _WORD_32_ * sizeof(uint_32));
				Q.flag = 0;
			}
			else
			{
				printf("cnt_j : %d \t| NUM : %d \t| ECADD_Jacobian\n", cnt_j, NUM);
				memcpy(P.x, PsX[NUM], _WORD_32_ * sizeof(uint_32));
				memcpy(P.y, PsY[NUM], _WORD_32_ * sizeof(uint_32));
				memcpy(P.z, PsZ[NUM], _WORD_32_ * sizeof(uint_32));
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

void Fixed_Base_Comb_Method3					(ECJ* input,		cuint32* inconst,								ECJ* output)
{
	ECJ Q; Q.flag = 1;
	ECJ P; P.flag = 0;
	uint_32 TEMP2[_WORD_32_] = { 0, };
	uint_32 TEMP3[_WORD_32_] = { 0, };
	uint_32 INVERSE[_WORD_32_] = { 0, };

	ECJ EC_TEMP; EC_TEMP.flag = 0;
	uint_32 CONST[_WORD_32_] = { 0, }; CONST[0] = 1;
	
	memcpy(P.x, input->x, _WORD_32_ * sizeof(uint_32));
	memcpy(P.y, input->y, _WORD_32_ * sizeof(uint_32));
	memcpy(P.z, input->z, _WORD_32_ * sizeof(uint_32));

	for (int cnt_i = 0; cnt_i < 128; cnt_i++)
	{
		Left_To_Right_Binary_MUL_Jacobian(&P, CONST, &EC_TEMP);
		memcpy(PsX[cnt_i], EC_TEMP.x, _WORD_32_ * sizeof(uint_32));
		memcpy(PsY[cnt_i], EC_TEMP.y, _WORD_32_ * sizeof(uint_32));
		memcpy(PsZ[cnt_i], EC_TEMP.z, _WORD_32_ * sizeof(uint_32));
	}

	uint_32 count1 = 0;
	uint_32 count2 = 1;
	uint_32 CNT_J = 2;
	uint_32 CNT_X = 128;
	uint_32 CNT_I = 64;
	uint_32 CNT_P = 0;
	ECJ PT; PT.flag = 0;
	uint_32 START = 0;
	printf("START : %d\n", START);
	for (int cnt_k = 0; cnt_k < 7; cnt_k++)
	{
		printf("ROUND : %d\n", cnt_k);
		printf("count1 : %d \t| count2 : %d \n", count1, count2);

		CONST[count1] = 0; count1++;
		CONST[count2] = 1; count2++;
		for (int cnt_j = 0; cnt_j < CNT_J - 1; cnt_j++)
		{
			printf("CNT_J - 1 : %d \t| CNT_P : %d \t| CNT_X : %d \t| cnt_j : %d \n", CNT_J - 1, CNT_P, CNT_X, cnt_j);
			CNT_P = CNT_X * cnt_j;	
			for (int cnt_i = 0; cnt_i < CNT_I; cnt_i++)
			{
				printf("START : %d \t| %d + %d\n", START, cnt_i, CNT_P); START++;
				Left_To_Right_Binary_MUL_Jacobian(&P, CONST, &EC_TEMP);
				ECADD_Jacobian_INPUT3(&PsX[cnt_i + CNT_P], PsY[cnt_i + CNT_P], PsZ[cnt_i + CNT_P], &EC_TEMP, &EC_TEMP);
				memcpy(PsX[cnt_i + CNT_P], EC_TEMP.x, _WORD_32_ * sizeof(uint_32));
				memcpy(PsY[cnt_i + CNT_P], EC_TEMP.y, _WORD_32_ * sizeof(uint_32));
				memcpy(PsZ[cnt_i + CNT_P], EC_TEMP.z, _WORD_32_ * sizeof(uint_32));
			}
		}
		printf("cnt_j : %d \t| (CNT_J - 1) : %d \t| CNT_X : %d \n", (CNT_J - 1) * CNT_X, CNT_J - 1, CNT_X);
		printf("cnt_j : %d \t| CNT_X : %d \t| CNT_I : %d \n", CNT_X + CNT_I, CNT_X, CNT_I);
		for (int cnt_j = (CNT_J - 1) * CNT_X; cnt_j < (CNT_J - 1) * CNT_X + CNT_I; cnt_j++)
		{
			printf("START : %d \t| %d\n", START, cnt_j); START++;
			//Left_To_Right_Binary_MUL_Jacobian(&P, CONST, &EC_TEMP);
			memcpy(PsX[cnt_j], EC_TEMP.x, _WORD_32_ * sizeof(uint_32));
			memcpy(PsY[cnt_j], EC_TEMP.y, _WORD_32_ * sizeof(uint_32));
			memcpy(PsZ[cnt_j], EC_TEMP.z, _WORD_32_ * sizeof(uint_32));
		}
		CNT_X = CNT_X / 2;
		CNT_I = CNT_I / 2;
		CNT_J = 2 * CNT_J;
	}

	uchar_8 STRING[32] = { 0, };
	Divide_String_Equal_Parts(inconst, STRING);

	uchar_8 NUM = 1;
	int cnt_j = 0;

	for (cnt_j = 31; cnt_j >= 0; cnt_j--)
	{
		NUM = STRING[cnt_j];
		if (NUM != 0)
		{
			memcpy(Q.x, PsX[NUM], _WORD_32_ * sizeof(uint_32));
			memcpy(Q.y, PsY[NUM], _WORD_32_ * sizeof(uint_32));
			memcpy(Q.z, PsZ[NUM], _WORD_32_ * sizeof(uint_32));

			printf("N : %X t\ ST : %X \t cnt_i : %d\n", NUM, STRING[cnt_j], cnt_j);
			printf("P : \t"); for (int cnt_i = 7; cnt_i >= 0; cnt_i--)printf("%8X", PsX[NUM][cnt_i]); printf("\n");
			printf("P : \t"); for (int cnt_i = 7; cnt_i >= 0; cnt_i--)printf("%8X", PsY[NUM][cnt_i]); printf("\n");
			printf("P : \t"); for (int cnt_i = 7; cnt_i >= 0; cnt_i--)printf("%8X", PsZ[NUM][cnt_i]); printf("\n\n");
			Q.flag = 0;
			goto EXIT;
		}
	}

EXIT:
	for (int cnt_i = cnt_j-1; cnt_i >= 0; cnt_i--)
	{
		printf("N : %X t\ ST : %X \t cnt_i : %d\n", NUM, STRING[cnt_j], cnt_j);

		ECDBL_Jacobian(&Q, &Q);

		NUM = STRING[cnt_i];
		memcpy(P.x, PsX[NUM], _WORD_32_ * sizeof(uint_32));
		memcpy(P.y, PsY[NUM], _WORD_32_ * sizeof(uint_32));
		memcpy(P.z, PsZ[NUM], _WORD_32_ * sizeof(uint_32));

		printf("N : %X t\ ST : %X \t cnt_i : %d\n", NUM, STRING[cnt_i], cnt_i);
		printf("P : \t"); for (int CNT = 7; CNT >= 0; CNT--)printf("%8X", PsX[NUM][CNT]); printf("\n");
		printf("P : \t"); for (int CNT = 7; CNT >= 0; CNT--)printf("%8X", PsY[NUM][CNT]); printf("\n");
		printf("P : \t"); for (int CNT = 7; CNT >= 0; CNT--)printf("%8X", PsZ[NUM][CNT]); printf("\n");
		printf("Q : \t"); for (int CNT = 7; CNT >= 0; CNT--)printf("%8X", Q.x[CNT]); printf("\n");
		printf("Q : \t"); for (int CNT = 7; CNT >= 0; CNT--)printf("%8X", Q.y[CNT]); printf("\n");
		printf("Q : \t"); for (int CNT = 7; CNT >= 0; CNT--)printf("%8X", Q.z[CNT]); printf("\n\n");

		if (NUM != 0)
			ECADD_Jacobian(&Q, &P, &Q);
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

void Fixed_Base_Comb_Method2					(ECJ* input,		cuint32* inconst,								ECJ* output)
{
	ECJ Q; Q.flag = 1;
	ECJ P; P.flag = 0;
	uint_32 TEMP2[_WORD_32_] = { 0, };
	uint_32 TEMP3[_WORD_32_] = { 0, };
	uint_32 INVERSE[_WORD_32_] = { 0, };

	ECJ EC_TEMP; EC_TEMP.flag = 0;
	uint_32 CONST[_WORD_32_] = { 0, }; CONST[0] = 0;
	
	memcpy(P.x, input->x, _WORD_32_ * sizeof(uint_32));
	memcpy(P.y, input->y, _WORD_32_ * sizeof(uint_32));
	memcpy(P.z, input->z, _WORD_32_ * sizeof(uint_32));

	if (Check_Num0(PsZ[0]) == _SUCCESS_)
	{
		for (int cnt_i = 0; cnt_i < 256; cnt_i++)
		{
			Left_To_Right_Binary_MUL_Jacobian(&P, CONST, &EC_TEMP);
			memcpy(PsX[cnt_i], EC_TEMP.x, _WORD_32_ * sizeof(uint_32));
			memcpy(PsY[cnt_i], EC_TEMP.y, _WORD_32_ * sizeof(uint_32));
			memcpy(PsZ[cnt_i], EC_TEMP.z, _WORD_32_ * sizeof(uint_32));
			CONST[0] = CONST[0] + 1;
		//	if (cnt_i == 0)
		//	{
		//		printf("P0 : \t"); for (int cnt_i = 7; cnt_i >= 0; cnt_i--)printf("%8X", PsX[0][cnt_i]); printf("\n");
		//		printf("P0 : \t"); for (int cnt_i = 7; cnt_i >= 0; cnt_i--)printf("%8X", PsY[0][cnt_i]); printf("\n");
		//		printf("P0 : \t"); for (int cnt_i = 7; cnt_i >= 0; cnt_i--)printf("%8X", PsZ[0][cnt_i]); printf("\n");
		//	}
		//	if (cnt_i == 1)
		//	{
		//		printf("P1 : \t"); for (int cnt_i = 7; cnt_i >= 0; cnt_i--)printf("%8X", PsX[1][cnt_i]); printf("\n");
		//		printf("P1 : \t"); for (int cnt_i = 7; cnt_i >= 0; cnt_i--)printf("%8X", PsY[1][cnt_i]); printf("\n");
		//		printf("P1 : \t"); for (int cnt_i = 7; cnt_i >= 0; cnt_i--)printf("%8X", PsZ[1][cnt_i]); printf("\n");
		//	}
		}
	}

	uchar_8 STRING[32] = { 0, };
	Divide_String_Equal_Parts(inconst, STRING);

	uchar_8 NUM = 1;
	int cnt_j = 0;

	for (cnt_j = 31; cnt_j >= 0; cnt_j--)
	{
		NUM = STRING[cnt_j];
		if (NUM != 0)
		{
			memcpy(Q.x, PsX[NUM], _WORD_32_ * sizeof(uint_32));
			memcpy(Q.y, PsY[NUM], _WORD_32_ * sizeof(uint_32));
			memcpy(Q.z, PsZ[NUM], _WORD_32_ * sizeof(uint_32));

			printf("N : %X t\ ST : %X \t cnt_i : %d\n", NUM, STRING[cnt_j], cnt_j);
			printf("P : \t"); for (int cnt_i = 7; cnt_i >= 0; cnt_i--)printf("%8X", PsX[NUM][cnt_i]); printf("\n");
			printf("P : \t"); for (int cnt_i = 7; cnt_i >= 0; cnt_i--)printf("%8X", PsY[NUM][cnt_i]); printf("\n");
			printf("P : \t"); for (int cnt_i = 7; cnt_i >= 0; cnt_i--)printf("%8X", PsZ[NUM][cnt_i]); printf("\n\n");
			Q.flag = 0;
			goto EXIT;
		}
	}

EXIT:
	for (int cnt_i = cnt_j-1; cnt_i >= 0; cnt_i--)
	{
		ECDBL_Jacobian(&Q, &Q);

		NUM = STRING[cnt_i];
		memcpy(P.x, PsX[NUM], _WORD_32_ * sizeof(uint_32));
		memcpy(P.y, PsY[NUM], _WORD_32_ * sizeof(uint_32));
		memcpy(P.z, PsZ[NUM], _WORD_32_ * sizeof(uint_32));

		printf("N : %X t\ ST : %X \t cnt_i : %d\n", NUM, STRING[cnt_i], cnt_i);
		printf("P : \t"); for (int CNT = 7; CNT >= 0; CNT--)printf("%8X", PsX[NUM][CNT]); printf("\n");
		printf("P : \t"); for (int CNT = 7; CNT >= 0; CNT--)printf("%8X", PsY[NUM][CNT]); printf("\n");
		printf("P : \t"); for (int CNT = 7; CNT >= 0; CNT--)printf("%8X", PsZ[NUM][CNT]); printf("\n");
		printf("Q : \t"); for (int CNT = 7; CNT >= 0; CNT--)printf("%8X", Q.x[CNT]); printf("\n");
		printf("Q : \t"); for (int CNT = 7; CNT >= 0; CNT--)printf("%8X", Q.y[CNT]); printf("\n");
		printf("Q : \t"); for (int CNT = 7; CNT >= 0; CNT--)printf("%8X", Q.z[CNT]); printf("\n\n");

		ECADD_Jacobian(&Q, &P, &Q);
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