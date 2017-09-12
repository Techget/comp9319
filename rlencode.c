#include <stdio.h>

// Define unsigned int size const
const int UINT_SIZE = 4;

void OneArg(char* file_in);
void TwoArg(char* file_in, char* file_out);
void Transform(unsigned int counter, unsigned char * transformed);

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
*   Used to handle one arguement situation, simply output the desired
*   compression in plain text.
*/
void OneArg(char* file_in){
	FILE* fpIn = fopen(file_in, "ro");

	char currentChar;
	char previousChar;
	int counter = 1;
	int firstFlag = 1;

	while((currentChar = fgetc(fpIn)) != EOF) {
		if(firstFlag){
			previousChar = currentChar;
			firstFlag = 0;
			continue;
		}

		if(currentChar == previousChar){
			counter++;
			continue;
		}else{
			if(counter>=3){
				counter -= 3;
				printf("%c[%d]",previousChar,counter);
			}else{
				while(counter>0){
					printf("%c",previousChar);
					counter--;
				}
			}
			previousChar = currentChar;
			counter = 1;
		}
	}

	if(counter>=3){
		counter -= 3;
		printf("%c[%d]",previousChar,counter);
	}else{
		while(counter>0){
			printf("%c",previousChar);
			counter--;
		}
	}

	fclose(fpIn);
}

/**
*	Handle two arguments situation. Compress the file and
*   output to the target file in binary format.
*/
void TwoArg(char* file_in, char* file_out){
	FILE* fpIn = fopen(file_in, "ro");
	FILE* fpOut = fopen(file_out, "wb");

	char ch;
	unsigned char currentChar;
	unsigned char previousChar[1];
	// used for the first input character
	previousChar[0] = 128;
	unsigned int counter = 1;
	unsigned char transformed[UINT_SIZE];

	while((ch = fgetc(fpIn)) != EOF) {
		// Do in this way, since EOF is singed, usually -1
		currentChar = (unsigned char)ch;
		// Special condition for the first read character
		if(previousChar[0] == 128){
			previousChar[0] = currentChar;
			continue;
		}

		if(currentChar == previousChar[0]){
			counter++;
		}else{
			if(counter>=3){
				fwrite(previousChar , sizeof(unsigned char) , 1 , fpOut);

				counter -= 3;
				Transform(counter, transformed);

				int i;
				// Read from high significant bit to low significant bit, so the
				// number 128 in the beginning can be ignored
				int removedHighOrderRedundantBitsFlag = 1;
				// Put more siginficant bit into output bit string first.
				for(i=3; i>=0; i--){
					if(transformed[i] == 128 && removedHighOrderRedundantBitsFlag){
						continue;
					}else{
						removedHighOrderRedundantBitsFlag = 0;
					}

					previousChar[0] = transformed[i];
					fwrite(previousChar, sizeof(unsigned char), 1, fpOut);
				}

				if(transformed[0]==128 && transformed[1]==128 
					&& transformed[2]==128 && transformed[3]==128){
					previousChar[0] = (unsigned char)128;
					fwrite(previousChar, sizeof(unsigned char), 1, fpOut);
				}
			}else{
				while(counter>0){
					fwrite(previousChar, sizeof(unsigned char), 1, fpOut);
					counter--;
				}
			}

			counter = 1;
			previousChar[0] = currentChar;
		}
	}

	// Output the character and corresponding counter before EOF to 
	// the target output file.
	if(counter>=3){
		fwrite(previousChar , sizeof(unsigned char) , 1 , fpOut);

		counter -= 3;

		Transform(counter, transformed);

		int i;
		int removedHighOrderRedundantBitsFlag = 1;
		for(i=3; i>=0; i--){
			if(transformed[i] == 128 && removedHighOrderRedundantBitsFlag){
				continue;
			}else{
				removedHighOrderRedundantBitsFlag = 0;
			}

			previousChar[0] = transformed[i];
			fwrite(previousChar, sizeof(unsigned char), 1, fpOut);
		}
	}else{
		while(counter>0){
			fwrite(previousChar, sizeof(unsigned char), 1, fpOut);
			counter--;
		}
	}

	fclose(fpIn);
	fclose(fpOut);
}

// Split integer counter into four byte, but haven't removed the redundant zeros in
// significant bit. Put more significant bit in higher index
void Transform(unsigned int counter, unsigned char * transformed){
	unsigned int c = counter;

	int i=0;
	for(i=0; i<UINT_SIZE; i++){
		// save the last 7 bit	
		unsigned char uch = (unsigned char)(c & 0x7F);
		uch = uch | 0x80;
		transformed[i] = uch;
		c >>= 7;		
	}	
}