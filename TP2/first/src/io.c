#include "io.h"
#include <string.h>
#include <stdlib.h>


void get_input_size(int *input_size){

	FILE *fileptr = popen("wc -c < ./input", "r");

	fscanf(fileptr, "%d", input_size);

	fclose(fileptr);
}

void load_input(char **input_buffer, int input_size){

	FILE *fileptr = fopen("./input", "r");

	fread(input_buffer, sizeof(char), input_size, fileptr);

	fclose(fileptr);
}

void load_ecosystem_specs(
	char* input_buffer,
	int *GEN_PROC_PREY,
	int *GEN_PROC_PREDATOR,
	int *GEN_PREDATOR_FOOD,
	int *N_GEN, 
	int *L, int *C, int *N
){

	char c;
	int p = 0;

	for (int i = 0; i < 7; i++){
		char string[3] = "\0";
		while((c = input_buffer[p], c != ' ' && c != '\n')){
			strcat(string, &c);
			p++;
		}
		if (i == 0) *GEN_PROC_PREY 		= atoi(string);
		if (i == 1) *GEN_PROC_PREDATOR 	= atoi(string);
		if (i == 2) *GEN_PREDATOR_FOOD 	= atoi(string);
		if (i == 3) *N_GEN 				= atoi(string);
		if (i == 4) *L 					= atoi(string);
		if (i == 5) *C 					= atoi(string);
		if (i == 6) *N 					= atoi(string);
		p++;
	}
}
