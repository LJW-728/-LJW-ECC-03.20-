#include "Basic_Definitions.h"
#include "Basic_Functions.h"
#include "Prime_Field_Arithmetic.h"
#include "Elliptic_Curve_Operations_using_Affine_Coordinate.h"
#include "Elliptic_Curve_Operations_using_Jacobian_Coordinate.h"
#include "Various_Scalar_Multiplications.h"

int64_t cpucycles(void)
{
	return __rdtsc();
}

int main()
{
	char RorW_R[] = "rb", RorW_W[] = "wb";
	char TRASH[4] = { 0, };

	uint_32 A[_WORD_32_] = { 0, };
	uint_32 B[_WORD_32_] = { 0, };
	uint_32 C[_WORD_32_] = { 0, };
	uint_32 A2[2 * _WORD_32_] = { 0, };
	uint_32 B2[2 * _WORD_32_] = { 0, };
	uint_32 C2[2 * _WORD_32_] = { 0, };

	uint_32  BasePointX[_WORD_32_] = { 0Xd898c296, 0Xf4a13945, 0X2deb33a0, 0X77037d81, 0X63a440f2, 0Xf8bce6e5, 0Xe12c4247, 0X6b17d1f2 };
	uint_32  BasePointY[_WORD_32_] = { 0X37bf51f5, 0Xcbb64068, 0X6b315ece, 0X2bce3357, 0X7c0f9e16, 0X8ee7eb4a, 0Xfe1a7f9b, 0X4fe342e2 };
	uint_32  BasePointZ[_WORD_32_] = { 0X00000001, 0X00000000, 0X00000000, 0X00000000, 0X00000000, 0X00000000, 0X00000000, 0X00000000 };

	ECA AA;
	ECA CC;
	ECJ AAA;
	ECJ CCC;

	memcpy(AA.x, BasePointX, _WORD_32_ * sizeof(uint_32));
	memcpy(AA.y, BasePointY, _WORD_32_ * sizeof(uint_32));

	memcpy(AAA.x, BasePointX, _WORD_32_ * sizeof(uint_32));
	memcpy(AAA.y, BasePointY, _WORD_32_ * sizeof(uint_32));
	memcpy(AAA.z, BasePointZ, _WORD_32_ * sizeof(uint_32));

	uint_64 CMOS_11 = 0, CMOS_12 = 0, CMOS_13 = 0;
	uint_64 CMOS_21 = 0, CMOS_22 = 0, CMOS_23 = 0;
	uint_64 CMOS_31 = 0, CMOS_32 = 0, CMOS_33 = 0;
	uint_64 CMOS_41 = 0, CMOS_42 = 0, CMOS_43 = 0;
	uint_64 CMOS_51 = 0, CMOS_52 = 0, CMOS_53 = 0;

	FILE* IN_FP_A = NULL;
	FILE* IN_FP_B = NULL;
	FILE* IN_FP_C = NULL;
	FILE* OUT_FP_A = NULL;
	FILE* OUT_FP_B = NULL;
	FILE* OUT_FP_C = NULL;
	FILE* OUT_FP_D = NULL;
	FILE* OUT_FP_E = NULL;



	char FN_INFP_W4_A[] = "W4_IN_TV_opA_ADD_SUB.txt";
	char FN_INFP_W4_B[] = "W4_IN_TV_opB_ADD_SUB.txt";
	fopen_s(&IN_FP_A, FN_INFP_W4_A, RorW_R); ERROR_HANDLING(IN_FP_A, FN_INFP_W4_A);
	fopen_s(&IN_FP_B, FN_INFP_W4_B, RorW_R); ERROR_HANDLING(IN_FP_B, FN_INFP_W4_B);

	char FN_W4_BIGNUM_ADD_64_RE[] = "W4_OUT_BIGNUM_ADD_64_RE.txt"; 
	char FN_W4_BIGNUM_SUB_64_RE[] = "W4_OUT_BIGNUM_SUB_64_RE.txt"; 
	fopen_s(&OUT_FP_A, FN_W4_BIGNUM_ADD_64_RE, RorW_W); ERROR_HANDLING(OUT_FP_A, FN_W4_BIGNUM_ADD_64_RE);
	fopen_s(&OUT_FP_B, FN_W4_BIGNUM_SUB_64_RE, RorW_W); ERROR_HANDLING(OUT_FP_B, FN_W4_BIGNUM_SUB_64_RE);

	while (!feof(IN_FP_A))
	{
		GET_INPUTBYTE_32(A, IN_FP_A, _WORD_32_);
		GET_INPUTBYTE_32(B, IN_FP_B, _WORD_32_);

		CMOS_11 = cpucycles(); BIGNUM_ADD_64_RE(A, B, C); CMOS_12 = cpucycles(); CMOS_13 += (CMOS_12 - CMOS_11); 
		PUT_ONE_INPUTBYTE_32(C, OUT_FP_A, _WORD_32_); memset(C, 0, _WORD_32_);

		CMOS_21 = cpucycles(); BIGNUM_SUB_64_RE(A, B, C); CMOS_22 = cpucycles(); CMOS_23 += (CMOS_22 - CMOS_21);
		PUT_ONE_INPUTBYTE_32(C, OUT_FP_B, _WORD_32_); memset(C, 0, _WORD_32_);

		fread(TRASH, 4, 1, IN_FP_A);
	}
	printf("BIGNUM_ADD_64_RE\t\t\t|%10lld\t|\n", CMOS_13 / 10000);
	printf("BIGNUM_SUB_64_RE\t\t\t|%10lld\t|\n", CMOS_23 / 10000);
	printf("\n");



	char FN_INFP_W5_A[] = "W5_IN_TV_opA_MUL_SQR.txt";
	char FN_INFP_W5_B[] = "W5_IN_TV_opB_MUL_SQR.txt";
	fopen_s(&IN_FP_A, FN_INFP_W5_A, RorW_R); ERROR_HANDLING(IN_FP_A, FN_INFP_W5_A);
	fopen_s(&IN_FP_B, FN_INFP_W5_B, RorW_R); ERROR_HANDLING(IN_FP_B, FN_INFP_W5_B);

	char FN_W5_BIGNUM_MUL_OS_64[] = "W5_OUT_BIGNUM_MUL_OS_64.txt";
	char FN_W5_BIGNUM_SQU_OS_64[] = "W5_OUT_BIGNUM_SQU_OS_64.txt";
	fopen_s(&OUT_FP_A, FN_W5_BIGNUM_MUL_OS_64, RorW_W); ERROR_HANDLING(OUT_FP_A, FN_W5_BIGNUM_MUL_OS_64);
	fopen_s(&OUT_FP_B, FN_W5_BIGNUM_SQU_OS_64, RorW_W); ERROR_HANDLING(OUT_FP_B, FN_W5_BIGNUM_SQU_OS_64);

	while (!feof(IN_FP_A))
	{
		GET_INPUTBYTE_32(A, IN_FP_A, _WORD_32_);
		GET_INPUTBYTE_32(B, IN_FP_B, _WORD_32_);

		CMOS_11 = cpucycles(); BIGNUM_MUL_OS_64(A, A, C2); CMOS_12 = cpucycles(); CMOS_13 += (CMOS_12 - CMOS_11);
		PUT_ONE_INPUTBYTE_32(C2, OUT_FP_A, 2 * _WORD_32_); memset(C2, 0, 2 * _WORD_32_);

		CMOS_21 = cpucycles(); BIGNUM_SQU_OS_64(A, C2); CMOS_22 = cpucycles(); CMOS_23 += (CMOS_22 - CMOS_21);
		PUT_ONE_INPUTBYTE_32(C2, OUT_FP_B, 2 * _WORD_32_); memset(C2, 0, 2 * _WORD_32_);

		fread(TRASH, 4, 1, IN_FP_A);
	}
	printf("BIGNUM_MUL_OS_64\t\t\t|%10lld\t|\n", CMOS_13 / 10000);
	printf("BIGNUM_SQU_OS_64\t\t\t|%10lld\t|\n", CMOS_23 / 10000);
	printf("\n");


	
	char FN_INFP_W6_A[] = "W6_IN_TV_MUL_TV.txt";
	char FN_INFP_W6_B[] = "W6_IN_TV_opA.txt";
	char FN_INFP_W6_C[] = "W6_IN_TV_opB.txt";
	fopen_s(&IN_FP_A, FN_INFP_W6_A, RorW_R); ERROR_HANDLING(IN_FP_A, FN_INFP_W6_A);
	fopen_s(&IN_FP_B, FN_INFP_W6_B, RorW_R); ERROR_HANDLING(IN_FP_B, FN_INFP_W6_B);
	fopen_s(&IN_FP_C, FN_INFP_W6_C, RorW_R); ERROR_HANDLING(IN_FP_C, FN_INFP_W6_C);

	char FN_W6_Reduction_64[]	   = "W6_OUT_Reduction_64.txt";
	char FN_W6_Fast_Reduction_64[] = "W6_OUT_Fast_Reduction_64.txt";
	fopen_s(&OUT_FP_A, FN_W6_Reduction_64, RorW_W);		 ERROR_HANDLING(OUT_FP_A, FN_W6_Reduction_64);
	fopen_s(&OUT_FP_B, FN_W6_Fast_Reduction_64, RorW_W); ERROR_HANDLING(OUT_FP_B, FN_W6_Fast_Reduction_64);

	char FN_W6_BIGNUM_MUL_OS_64_RE[] = "W6_OUT_BIGNUM_MUL_OS_64_RE.txt";
	char FN_W6_BIGNUM_SQU_OS_64_RE[] = "W6_OUT_BIGNUM_SQU_OS_64_RE.txt";
	fopen_s(&OUT_FP_C, FN_W6_BIGNUM_MUL_OS_64_RE, RorW_W); ERROR_HANDLING(OUT_FP_C, FN_W6_BIGNUM_MUL_OS_64_RE);
	fopen_s(&OUT_FP_D, FN_W6_BIGNUM_SQU_OS_64_RE, RorW_W); ERROR_HANDLING(OUT_FP_D, FN_W6_BIGNUM_SQU_OS_64_RE);

	while (!feof(IN_FP_A))
	{
		GET_INPUTBYTE_32(A2, IN_FP_A, 2 * _WORD_32_);
		GET_INPUTBYTE_32(A, IN_FP_B, _WORD_32_);
		GET_INPUTBYTE_32(B, IN_FP_C, _WORD_32_);

		CMOS_11 = cpucycles(); Reduction_64(A2, C); CMOS_12 = cpucycles(); CMOS_13 += (CMOS_12 - CMOS_11);
		PUT_ONE_INPUTBYTE_32(C, OUT_FP_A, _WORD_32_); memset(C, 0, _WORD_32_);

		CMOS_21 = cpucycles(); Fast_Reduction_64(A2, C); CMOS_22 = cpucycles(); CMOS_23 += (CMOS_22 - CMOS_21);
		PUT_ONE_INPUTBYTE_32(C, OUT_FP_B, _WORD_32_); memset(C, 0, _WORD_32_);

		CMOS_31 = cpucycles(); BIGNUM_MUL_OS_64_RE(A, B, C); CMOS_32 = cpucycles(); CMOS_33 += (CMOS_32 - CMOS_31);
		PUT_ONE_INPUTBYTE_32(C, OUT_FP_C, _WORD_32_); memset(C, 0, _WORD_32_);

		CMOS_41 = cpucycles(); BIGNUM_SQU_OS_64_RE(A, C); CMOS_42 = cpucycles(); CMOS_43 += (CMOS_42 - CMOS_41);
		PUT_ONE_INPUTBYTE_32(C, OUT_FP_D, _WORD_32_); memset(C, 0, _WORD_32_);

		fread(TRASH, 4, 1, IN_FP_A);
	}
	printf("Reduction_64\t\t\t\t|%10lld\t|\n",			CMOS_13 / 10000);
	printf("Fast_Reduction_64\t\t\t|%10lld\t|\n",		CMOS_23 / 10000);
	printf("BIGNUM_MUL_OS_64_RE\t\t\t|%10lld\t|\n",		CMOS_33 / 10000);
	printf("BIGNUM_SQU_OS_64_RE\t\t\t|%10lld\t|\n",		CMOS_43 / 10000);
	printf("\n");



	char FN_INFP_W7_A[] = "W7_IN_TV_opA.txt";
	fopen_s(&IN_FP_A, FN_INFP_W7_A, RorW_R); ERROR_HANDLING(IN_FP_A, FN_INFP_W7_A);

	char FN_W7_Fermat_Based_Inversion[]	  = "W7_OUT_Fermat_Based_Inversion.txt";
	char FN_W7_Binary_Alg_Inversion_Alg[] = "W7_OUT_Binary_Alg_Inversion_Alg.txt";
	fopen_s(&OUT_FP_A, FN_W7_Fermat_Based_Inversion, RorW_W);	ERROR_HANDLING(OUT_FP_A, FN_W7_Fermat_Based_Inversion);
	fopen_s(&OUT_FP_B, FN_W7_Binary_Alg_Inversion_Alg, RorW_W); ERROR_HANDLING(OUT_FP_B, FN_W7_Binary_Alg_Inversion_Alg);

	while (!feof(IN_FP_A))
	{
		GET_INPUTBYTE_32(A, IN_FP_A, _WORD_32_);

		CMOS_11 = cpucycles(); Fermat_Based_Inversion(A, C); CMOS_12 = cpucycles(); CMOS_13 += (CMOS_12 - CMOS_11);
		PUT_ONE_INPUTBYTE_32(C, OUT_FP_A, _WORD_32_); memset(C, 0, _WORD_32_);

		CMOS_21 = cpucycles(); Binary_Alg_Inversion_Alg(A, C); CMOS_22 = cpucycles(); CMOS_23 += (CMOS_22 - CMOS_21);
		PUT_ONE_INPUTBYTE_32(C, OUT_FP_B, _WORD_32_); memset(C, 0, _WORD_32_);

		fread(TRASH, 4, 1, IN_FP_A);
	}
	printf("Fermat_Based_Inversion\t\t\t|%10lld\t|\n", CMOS_13 / 10000);
	printf("Binary_Alg_Inversion_Alg\t\t|%10lld\t|\n", CMOS_23 / 10000);
	printf("\n");



	char FN_INFP_W8_A[] = "W8_IN_TV_Scalar.txt";
	fopen_s(&IN_FP_A, FN_INFP_W8_A, RorW_R); ERROR_HANDLING(IN_FP_A, FN_INFP_W8_A);

	char FN_W8_Left_To_Right_Binary_MUL_Affine[]		= "W8_OUT_Left_To_Right_Binary_MUL_Affine.txt";
	char FN_W8_Right_To_Left_Binary_MUL_Affine[]		= "W8_OUT_Right_To_Left_Binary_MUL_Affine.txt";
	char FN_W8_Left_To_Right_Binary_MUL_Jacobian[]		= "W8_OUT_Left_To_Right_Binary_MUL_Jacobian.txt";
	char FN_W9_Window_NAF_Method_for_Multiplication[]	= "W9_OUT_Window_NAF_Method_for_Multiplication.txt";
	char FN_W9_Fixed_Base_Comb_Method[]					= "W9_OUT_Fixed_Base_Comb_Method.txt";
	fopen_s(&OUT_FP_A, FN_W8_Left_To_Right_Binary_MUL_Affine, RorW_W);		ERROR_HANDLING(OUT_FP_A, FN_W8_Left_To_Right_Binary_MUL_Affine);
	fopen_s(&OUT_FP_B, FN_W8_Right_To_Left_Binary_MUL_Affine, RorW_W);		ERROR_HANDLING(OUT_FP_B, FN_W8_Right_To_Left_Binary_MUL_Affine);
	fopen_s(&OUT_FP_C, FN_W8_Left_To_Right_Binary_MUL_Jacobian, RorW_W);	ERROR_HANDLING(OUT_FP_C, FN_W8_Left_To_Right_Binary_MUL_Jacobian);
	fopen_s(&OUT_FP_D, FN_W9_Window_NAF_Method_for_Multiplication, RorW_W);	ERROR_HANDLING(OUT_FP_D, FN_W9_Window_NAF_Method_for_Multiplication);
	fopen_s(&OUT_FP_E, FN_W9_Fixed_Base_Comb_Method, RorW_W);				ERROR_HANDLING(OUT_FP_E, FN_W9_Fixed_Base_Comb_Method);
	int i = 0;
	uchar_8 TEMP[32] = { 0, };
	while (!feof(IN_FP_A))
	{
		printf("%.4d\n", i); i++;
		GET_INPUTBYTE_32(A, IN_FP_A, _WORD_32_);

		CMOS_11 = cpucycles(); Left_To_Right_Binary_MUL_Affine(&AA, A, &CC); CMOS_12 = cpucycles(); CMOS_13 += (CMOS_12 - CMOS_11);
		PUT_TWO_INPUTBYTE_32(CC.x, CC.y, OUT_FP_A, _WORD_32_); memset(CC.x, 0, _WORD_32_); memset(CC.y, 0, _WORD_32_);

		CMOS_21 = cpucycles(); Right_To_Left_Binary_MUL_Affine(&AA, A, &CC); CMOS_22 = cpucycles(); CMOS_23 += (CMOS_22 - CMOS_21);
		PUT_TWO_INPUTBYTE_32(CC.x, CC.y, OUT_FP_B, _WORD_32_); memset(CC.x, 0, _WORD_32_); memset(CC.y, 0, _WORD_32_);

		CMOS_31 = cpucycles(); Left_To_Right_Binary_MUL_Jacobian(&AAA, A, &CCC); CMOS_32 = cpucycles(); CMOS_33 += (CMOS_32 - CMOS_31);
		PUT_TWO_INPUTBYTE_32(CCC.x, CCC.y, OUT_FP_C, _WORD_32_); memset(CCC.x, 0, _WORD_32_); memset(CCC.y, 0, _WORD_32_); memset(CCC.z, 0, _WORD_32_);

		CMOS_41 = cpucycles(); NAF_Method_for_Multiplication(&AAA, A, 4, &CCC); CMOS_42 = cpucycles(); CMOS_43 += (CMOS_42 - CMOS_41);
		PUT_TWO_INPUTBYTE_32(CCC.x, CCC.y, OUT_FP_D, _WORD_32_); memset(CCC.x, 0, _WORD_32_); memset(CCC.y, 0, _WORD_32_); memset(CCC.z, 0, _WORD_32_);

		CMOS_51 = cpucycles(); Fixed_Base_Comb_Method(&AAA, A, &CCC); CMOS_52 = cpucycles(); CMOS_53 += (CMOS_52 - CMOS_51);
		PUT_TWO_INPUTBYTE_32(CCC.x, CCC.y, OUT_FP_E, _WORD_32_); memset(CCC.x, 0, _WORD_32_); memset(CCC.y, 0, _WORD_32_); memset(CCC.z, 0, _WORD_32_);

		fread(TRASH, 4, 1, IN_FP_A);
	}
	printf("Left_To_Right_Binary_MUL_Affine\t\t|%10lld\t|\n",		CMOS_13 / 10000);
	printf("Right_To_Left_Binary_MUL_Affine\t\t|%10lld\t|\n",		CMOS_23 / 10000);
	printf("Left_To_Right_Binary_MUL_Jacobian\t|%10lld\t|\n",		CMOS_33 / 10000);
	printf("Window_NAF_Method_for_Multiplication\t|%10lld\t|\n",	CMOS_43 / 10000);
	printf("Fixed_Base_Comb_Method\t\t\t|%10lld\t|\n",				CMOS_53 / 10000);
	printf("\n");




}