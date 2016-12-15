#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if(argc < 3) {
		printf("Please enter an input file and an output file.");
	} else {
		FILE* input  = fopen(argv[1], "r");
		FILE* output = fopen(argv[2], "w");
		char byte[3];
		byte[2] = '\0';
		while(!feof(input)) {
			int byte1 = fgetc(input);
			while(byte1 == ' ' || byte1 == '\n' || byte1 == '\r') {
				byte1 = fgetc(input);
			}
			byte[0] = byte1;
			byte[1] = fgetc(input);
			int value = (int)strtol(byte, NULL, 16);
			fputc(value, output);
		}
		fclose(input);
		fclose(output);
	}
}
