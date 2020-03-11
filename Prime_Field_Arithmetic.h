#pragma once

#include <memory.h>						// memcpy

//	* �Լ���	: BIGNUM_ADD_64
//	* ���� 		: (256bit ������) �� ���� 64bit ������ ���ϰ� ����� 2^256���� ū �������� �˷��ִ� �Լ��̴�.
/*
	// parameter
	* input1	: (���ϰ� ����) 256bit ������ ��
	* input2	: (���ϰ� ����) 256bit ������ ��
	* output	: (257bit�� ������) 256bit ������ ���

	// return
	1			: (����) ����� 2^256 �̻�
	0			: (����) ����� 2^256 �̸�
*/
int  BIGNUM_ADD_64						(cuint32* input1,		cuint32* input2,			uint_32* output);

//	* �Լ���	: BIGNUM_SUB_64
//	* ���� 		: (256bit ������) �� ���� 64bit ������ ���� ����� ���������� �˷��ִ� �Լ��̴�.
/*
	// parameter
	* input1	: (���� ����) 256bit ������ ��
	* input2	: (���� ����) 256bit ������ ��
	* output	: (257bit�� ������) 256bit ������ ���

	// return
	1			: (��) ����� ����
	0			: (��) ����� ���
*/
int  BIGNUM_SUB_64						(cuint32* input1,		cuint32* input2,			uint_32* output);

//	* �Լ���	: BIGNUM_ADD_64_RE
//	* ���� 		: (256bit ������) �� ���� 64bit ������ ���ϰ� ����� P_256���� ��ⷯ�ϴ� �Լ��̴�.
/*
	// parameter
	* input1	: (���ϰ� ����) 256bit ������ ��
	* input2	: (���ϰ� ����) 256bit ������ ��
	* output	: (P_256���� ��ⷯ��) 256bit ������ ���
*/
void BIGNUM_ADD_64_RE					(cuint32* input1,		cuint32* input2,			uint_32* output);

//	* �Լ���	: BIGNUM_SUB_64_RE
//	* ���� 		: (256bit ������) �� ���� 64bit ������ ���� ����� P_256���� ��ⷯ�ϴ� �Լ��̴�.
/*
	// parameter
	* input1	: (���� ����) 256bit ������ ��
	* input2	: (���� ����) 256bit ������ ��
	* output	: (P_256���� ��ⷯ��) 256bit ������ ���
*/
void BIGNUM_SUB_64_RE					(cuint32* input1,		cuint32* input2,			uint_32* output);

//	* �Լ���	: Reduction_64
//	* ���� 		: (512bit ������) �Է°��� 64bit ������ P_256���� ��ⷯ�ϴ� �Լ��̴�.
/*
	// parameter
	* Linput	: (P_256���� ��ⷯ�ϰ� ����) 512bit ������ ��
	* output	: (P_256���� ��ⷯ��) 256bit ������ ���
*/
void Reduction_64						(cuint32* Linput,									uint_32* output);

//	* �Լ���	: Fast_Reduction_64
//	* ���� 		: (512bit ������) �Է°��� 64bit ������ P_256���� ��ⷯ�ϴ� �Լ��̴�.
/*
	// parameter
	* Linput	: (P_256���� ��ⷯ�ϰ� ����) 512bit ������ ��
	* output	: (P_256���� ��ⷯ��) 256bit ������ ���
*/
void Fast_Reduction_64					(cuint32* Linput,									uint_32* output);

//	* �Լ���	: BIGNUM_MUL_OS_64
//	* ���� 		: �Է°��� 64bit ������ ���ϴ� �Լ��̴�.
/*
	// parameter
	* input1	: (���ϰ� ����) 256bit ������ ��
	* input2	: (���ϰ� ����) 256bit ������ ��
	* Loutput	: 512bit ������ ���
*/
void BIGNUM_MUL_OS_64					(cuint32* input1,		cuint32* input2,			uint_32* output);

//	* �Լ���	: BIGNUM_SQU_OS_64
//	* ���� 		: �Է°��� 64bit ������ wp���ϴ� �Լ��̴�.
/*
	// parameter
	* input		: (�����ϰ� ����) 256bit ������ ��
	* Loutput	: 512bit ������ ���
*/
void BIGNUM_SQU_OS_64					(cuint32* input,									uint_32* output);

//	* �Լ���	: BIGNUM_MUL_OS_64_RE
//	* ���� 		: �Է°��� 64bit ������ ���ϰ� P_256���� ��ⷯ�ϴ� �Լ��̴�.
/*
	// parameter
	* input1	: (���ϰ� ����) 256bit ������ ��
	* input2	: (���ϰ� ����) 256bit ������ ��
	* output	: (P_256���� ��ⷯ��) 256bit ������ ���
*/
void BIGNUM_MUL_OS_64_RE				(cuint32* input1,		cuint32* input2,			uint_32* output);

//	* �Լ���	: BIGNUM_SQU_OS_64_RE
//	* ���� 		: �Է°��� 64bit ������ �����ϰ� P_256���� ��ⷯ�ϴ� �Լ��̴�.
/*
	// parameter
	* input		: (�����ϰ� ����) 256bit ������ ��
	* output	: (P_256���� ��ⷯ��) 256bit ������ ���
*/
void BIGNUM_SQU_OS_64_RE				(cuint32* input,									uint_32* output);

void Fermat_Based_Inversion				(cuint32* input,									uint_32* output);

int  Check_Num1							(cuint32* input);

int  Compare_Two_Numbers				(cuint32* input1,		cuint32* input2);

void Binary_Alg_Inversion_Alg			(cuint32* input,									uint_32* output);