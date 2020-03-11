#pragma once

#include <string.h>			// ERROR_HANDLING - strlen

//	* �Լ���	: ERROR_HANDLING
//	* ���� 		: ������ ���������� �������� Ȯ���ϴ� �Լ��̴�.
/*	
	// parameter
	* fp		: (Ȯ����) ���� ������
	* len		: (Ȯ����) �����̸��� ����� ����

	// return
	1			: ����
	0			: ����
*/
int  ERROR_HANDLING				(FILE* fp,				char* filename);

//	* �Լ���	: GET_INPUTBYTE_32
//	* ���� 		: 16������ �̷���� �����͸� 32��Ʈ ������ �о���� �Լ��̴�.
/*	
	// parameter
	* input		: �����͸� ������ ����
	* fp		: (�о��) ���� ������
	* len		: (�о��) ������ ����
*/
void GET_INPUTBYTE_32			(uint_32* input,		FILE* fp,			int len);

//	* �Լ���	: PUT_ONE_INPUTBYTE_32
//	* ���� 		: 16������ �̷���� �����͸� 32��Ʈ ������ �����ϴ� �Լ��̴�.
/*	
	// parameter
	* input		: �����Ͱ� ����� ����
	* fp		: (������) ���� ������
	* len		: (������) ������ ����
*/
void PUT_ONE_INPUTBYTE_32		(uint_32* input,		FILE* fp,			int len);

//	* �Լ���	: PUT_TWO_INPUTBYTE_32
//	* ���� 		: 16������ �̷���� �� ���� �����͸� 32��Ʈ ������ �����ϴ� �Լ��̴�.
/*
	// parameter
	* input1	: (ù ��°) �����Ͱ� ����� ����
	* input2	: (�� ��°) �����Ͱ� ����� ����
	* fp		: (������) ���� ������
	* len		: (������) ������ ����
*/
void PUT_TWO_INPUTBYTE_32		(uint_32* input1,		uint_32* input2,	FILE* fp,		int len);