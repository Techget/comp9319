#include <stdio.h>

const int UINT_SIZE = 4;
// Each time read 1 byte
const int NMEMB = 1;

void OneArg(char* file_in);
void TwoArg(char* file_in, char* file_out);
unsigned int Transform_From_Arr_To_Number(unsigned char transform_number[]);

int main (int argc, char *argv[]){
	if(argc == 2){
		OneArg(argv[1]);
	} else if(argc == 3){
		TwoArg(argv[1], argv[2]);
	} else{
		// printf("wrong input\n");
		return -1;
	}

	return 0;
}

/**
*	Deal the One argument situation, read from binary file and output the
*	desired compressed string in plain text.
*/

void OneArg(char* file_in){
	FILE* fpIn = fopen(file_in, "rb" );

	unsigned char buffer[NMEMB];
	unsigned char transform_number[UINT_SIZE];

	// Initialization for array transfor_number, make it all zero,
	// later, when decoding, 0 could be used to skip pointless step.
	// Use 0, since all the number should be larger than 128.
	int i;
	for(i=0;i<UINT_SIZE;i++){
		transform_number[i] = 0;
	}

	// flag vary from 0 to 3, since it could be an assertion that 
	// the corresponding number representation is four bytes at most 
	// when it's greater than 0, it indicate that we need to output the number.
	// It's also the index of next input unsigned char to put into 
	// the transform_number array.
	int outputNumberFlag = 0; 
	while(fread(buffer,sizeof(unsigned char),NMEMB,fpIn) > 0){
		if(buffer[0] < 127){
			if(outputNumberFlag){
				unsigned int counter = 
					Transform_From_Arr_To_Number(transform_number);
				printf("[%d]",counter);

				for(i=0;i<UINT_SIZE;i++){
					transform_number[i] = 0;
				}
				outputNumberFlag = 0;
			}

			// It's very tricky to put the printf after previous computation
			// Since the first of this file must be a ascii charater instead
			// of a number.
			printf("%c",(char)buffer[0]);
		}else{
			transform_number[outputNumberFlag] = buffer[0];
			outputNumberFlag++;
		}
	}

	// deal with edge condition, where the last output is a number
	if(outputNumberFlag){
		unsigned int counter = 
			Transform_From_Arr_To_Number(transform_number);
		printf("[%d]",counter);
	}

	fclose(fpIn);
}


/**
*	Deal with two arguments situation, mostly identical to OneArg function,
*	output will expand the counter, will recover to exactly what is input for
*	rlencode.
*/ 
void TwoArg(char* file_in, char* file_out){
	FILE* fpIn = fopen(file_in, "rb" );
	FILE* fpOut = fopen(file_out, "w+");

	unsigned char buffer[NMEMB];
	unsigned char transform_number[UINT_SIZE];

	int i;
	for(i=0;i<UINT_SIZE;i++){
		transform_number[i] = 0;
	}

	int outputNumberFlag = 0;
	unsigned char previousChar; 
	while(fread(buffer,sizeof(unsigned char),NMEMB,fpIn) > 0){
		if(buffer[0] < 127){
			if(outputNumberFlag){
				unsigned int counter = 
					Transform_From_Arr_To_Number(transform_number);
				counter += 3;
				// Here is gt 1 since there would be one certain output
				// for every character
				while(counter > 1){
					fprintf(fpOut,"%c", previousChar);
					counter--;
				}
				
				for(i=0;i<UINT_SIZE;i++){
					transform_number[i] = 0;
				}
				outputNumberFlag = 0;
			}

			fprintf(fpOut,"%c",(char)buffer[0]);
			previousChar = (char)buffer[0];
		}else{
			transform_number[outputNumberFlag] = buffer[0];
			outputNumberFlag++;
		}
	}

	// deal with edge condition, where the last output is a number, since
	// in above loop, if the last is the number it won't enter the if block
	// anymore.
	if(outputNumberFlag){
		unsigned int counter = 
			Transform_From_Arr_To_Number(transform_number);
		counter += 3;
		while(counter > 1){
			fprintf(fpOut,"%c", previousChar);
			counter--;
		}
	}

	fclose(fpIn);
	fclose(fpOut);
}

unsigned int Transform_From_Arr_To_Number(unsigned char transform_number[]){
	unsigned int output;
	unsigned char temp;

	// The first byte, no need for shifting
	// And this cover the edge condition where repeat number is 3
	// get the last 7 bit
	temp = transform_number[0] & 0x7F;
	output = (unsigned int)temp;

	int i;
	for(i=1;i<UINT_SIZE;i++){
		// Since in encode process, each number should gt 128.
		if(transform_number[i] == 0){
			break;
		}
		temp = transform_number[i] & 0x7F;
		output <<= 7;
		output = output | temp;
	}
	return output;
}

