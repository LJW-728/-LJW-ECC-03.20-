#pragma once

#include <memory.h>						// memcpy

//	* 함수명	: BIGNUM_ADD_64
//	* 설명 		: (256bit 길이의) 두 수를 64bit 단위로 더하고 결과가 2^256보다 큰 수인지를 알려주는 함수이다.
/*
	// parameter
	* input1	: (더하고 싶은) 256bit 단위의 수
	* input2	: (더하고 싶은) 256bit 단위의 수
	* output	: (257bit를 제외한) 256bit 단위의 결과

	// return
	1			: (더한) 결과가 2^256 이상
	0			: (더한) 결과가 2^256 미만
*/
int  BIGNUM_ADD_64						(cuint32* input1,		cuint32* input2,			uint_32* output);

//	* 함수명	: BIGNUM_SUB_64
//	* 설명 		: (256bit 길이의) 두 수를 64bit 단위로 빼고 결과가 음수인지를 알려주는 함수이다.
/*
	// parameter
	* input1	: (빼고 싶은) 256bit 단위의 수
	* input2	: (빼고 싶은) 256bit 단위의 수
	* output	: (257bit를 제외한) 256bit 단위의 결과

	// return
	1			: (뺀) 결과가 음수
	0			: (뺀) 결과가 양수
*/
int  BIGNUM_SUB_64						(cuint32* input1,		cuint32* input2,			uint_32* output);

//	* 함수명	: BIGNUM_ADD_64_RE
//	* 설명 		: (256bit 길이의) 두 수를 64bit 단위로 더하고 결과를 P_256으로 모듈러하는 함수이다.
/*
	// parameter
	* input1	: (더하고 싶은) 256bit 단위의 수
	* input2	: (더하고 싶은) 256bit 단위의 수
	* output	: (P_256으로 모듈러한) 256bit 단위의 결과
*/
void BIGNUM_ADD_64_RE					(cuint32* input1,		cuint32* input2,			uint_32* output);

//	* 함수명	: BIGNUM_SUB_64_RE
//	* 설명 		: (256bit 길이의) 두 수를 64bit 단위로 빼고 결과를 P_256으로 모듈러하는 함수이다.
/*
	// parameter
	* input1	: (빼고 싶은) 256bit 단위의 수
	* input2	: (빼고 싶은) 256bit 단위의 수
	* output	: (P_256으로 모듈러한) 256bit 단위의 결과
*/
void BIGNUM_SUB_64_RE					(cuint32* input1,		cuint32* input2,			uint_32* output);

//	* 함수명	: Reduction_64
//	* 설명 		: (512bit 단위의) 입력값을 64bit 단위로 P_256으로 모듈러하는 함수이다.
/*
	// parameter
	* Linput	: (P_256으로 모듈러하고 싶은) 512bit 단위의 수
	* output	: (P_256으로 모듈러한) 256bit 단위의 결과
*/
void Reduction_64						(cuint32* Linput,									uint_32* output);

//	* 함수명	: Fast_Reduction_64
//	* 설명 		: (512bit 단위의) 입력값을 64bit 단위로 P_256으로 모듈러하는 함수이다.
/*
	// parameter
	* Linput	: (P_256으로 모듈러하고 싶은) 512bit 단위의 수
	* output	: (P_256으로 모듈러한) 256bit 단위의 결과
*/
void Fast_Reduction_64					(cuint32* Linput,									uint_32* output);

//	* 함수명	: BIGNUM_MUL_OS_64
//	* 설명 		: 입력값을 64bit 단위로 곱하는 함수이다.
/*
	// parameter
	* input1	: (곱하고 싶은) 256bit 단위의 수
	* input2	: (곱하고 싶은) 256bit 단위의 수
	* Loutput	: 512bit 단위의 결과
*/
void BIGNUM_MUL_OS_64					(cuint32* input1,		cuint32* input2,			uint_32* output);

//	* 함수명	: BIGNUM_SQU_OS_64
//	* 설명 		: 입력값을 64bit 단위로 wp곱하는 함수이다.
/*
	// parameter
	* input		: (제곱하고 싶은) 256bit 단위의 수
	* Loutput	: 512bit 단위의 결과
*/
void BIGNUM_SQU_OS_64					(cuint32* input,									uint_32* output);

//	* 함수명	: BIGNUM_MUL_OS_64_RE
//	* 설명 		: 입력값을 64bit 단위로 곱하고 P_256으로 모듈러하는 함수이다.
/*
	// parameter
	* input1	: (곱하고 싶은) 256bit 단위의 수
	* input2	: (곱하고 싶은) 256bit 단위의 수
	* output	: (P_256으로 모듈러한) 256bit 단위의 결과
*/
void BIGNUM_MUL_OS_64_RE				(cuint32* input1,		cuint32* input2,			uint_32* output);

//	* 함수명	: BIGNUM_SQU_OS_64_RE
//	* 설명 		: 입력값을 64bit 단위로 제곱하고 P_256으로 모듈러하는 함수이다.
/*
	// parameter
	* input		: (제곱하고 싶은) 256bit 단위의 수
	* output	: (P_256으로 모듈러한) 256bit 단위의 결과
*/
void BIGNUM_SQU_OS_64_RE				(cuint32* input,									uint_32* output);

void Fermat_Based_Inversion				(cuint32* input,									uint_32* output);

int  Check_Num1							(cuint32* input);

int  Compare_Two_Numbers				(cuint32* input1,		cuint32* input2);

void Binary_Alg_Inversion_Alg			(cuint32* input,									uint_32* output);