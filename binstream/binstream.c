#include <stdio.h>
#include <stdlib.h>

#include "instructions.h"

int main(int argc, char *argv[]) {
	if(argc < 2) {
		printf("Please give an argument of a binary file to print.");
		return -1;
	} else {
		int start = 0;
		int end = -1;
		if(argc == 4) {
			start = atoi(argv[2]);
			end = atoi(argv[3]);
		}
		FILE* file = fopen(argv[1], "r");
		ubyte buffer[4];
		int index = 0;
		while(!feof(file) && (end == -1 || index <= end)) {
			fread(buffer, sizeof(ubyte), 4, file);
			if(index >= start) {
				printf("I%d:%d\t", index, get_number(buffer));
				if(index % 5 == 0)
					printf("\n");
			}
			index++;
		}
		printf("\n");
		return 0;
	}
}
