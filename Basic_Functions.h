#pragma once

#include <string.h>			// ERROR_HANDLING - strlen

//	* 함수명	: ERROR_HANDLING
//	* 설명 		: 파일이 정상적으로 열리는지 확인하는 함수이다.
/*	
	// parameter
	* fp		: (확인할) 파일 포인터
	* len		: (확인할) 파일이름이 저장된 변수

	// return
	1			: 성공
	0			: 실패
*/
int  ERROR_HANDLING				(FILE* fp,				char* filename);

//	* 함수명	: GET_INPUTBYTE_32
//	* 설명 		: 16진수로 이루어진 데이터를 32비트 단위로 읽어오는 함수이다.
/*	
	// parameter
	* input		: 데이터를 저장할 버퍼
	* fp		: (읽어올) 파일 포인터
	* len		: (읽어올) 데이터 길이
*/
void GET_INPUTBYTE_32			(uint_32* input,		FILE* fp,			int len);

//	* 함수명	: PUT_ONE_INPUTBYTE_32
//	* 설명 		: 16진수로 이루어진 데이터를 32비트 단위로 저장하는 함수이다.
/*	
	// parameter
	* input		: 데이터가 저장된 버퍼
	* fp		: (저장할) 파일 포인터
	* len		: (저장할) 데이터 길이
*/
void PUT_ONE_INPUTBYTE_32		(uint_32* input,		FILE* fp,			int len);

//	* 함수명	: PUT_TWO_INPUTBYTE_32
//	* 설명 		: 16진수로 이루어진 두 개의 데이터를 32비트 단위로 저장하는 함수이다.
/*
	// parameter
	* input1	: (첫 번째) 데이터가 저장된 버퍼
	* input2	: (두 번째) 데이터가 저장된 버퍼
	* fp		: (저장할) 파일 포인터
	* len		: (저장할) 데이터 길이
*/
void PUT_TWO_INPUTBYTE_32		(uint_32* input1,		uint_32* input2,	FILE* fp,		int len);