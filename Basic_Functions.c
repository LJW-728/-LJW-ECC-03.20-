#include "Basic_Definitions.h"
#include "Basic_Functions.h"

int  ERROR_HANDLING				(FILE* fp,				char* filename)
{
	if (fp == NULL) {
		printf("----------------------------------------------------------------\n");
		printf("\t| ");
		for (int cnt_i = 0; cnt_i < (int)strlen(filename); cnt_i++)
			printf("%c", filename[cnt_i]);
		printf(" |\tFILE OPEN ERROR\n");
		printf("----------------------------------------------------------------\n");
		return _FAILURE_;
	}

	return _SUCCESS_;
}

void GET_INPUTBYTE_32			(uint_32* input,		FILE* fp,			int len)
{
	uint_32 temp = 0;

	for (int cnt_i = 0; cnt_i < len; cnt_i++) {
		fscanf_s(fp, "%08X", &temp);
		input[len - cnt_i - 1] = (uint_32)temp;
	}
}

void PUT_ONE_INPUTBYTE_32		(uint_32* input,		FILE* fp,			int len)
{
	for (int cnt_i = len - 1; cnt_i >= 0; cnt_i--)
		fprintf(fp, "%08X", input[cnt_i]);
	fprintf(fp, "\n\n");
}

void PUT_TWO_INPUTBYTE_32		(uint_32* input1,		uint_32* input2,	FILE* fp,		int len)
{
	for (int cnt_i = len - 1; cnt_i >= 0; cnt_i--)
		fprintf(fp, "%08X", input1[cnt_i]);
	fprintf(fp, "\n");
	for (int cnt_i = len - 1; cnt_i >= 0; cnt_i--)
		fprintf(fp, "%08X", input2[cnt_i]);
	fprintf(fp, "\n\n");
}