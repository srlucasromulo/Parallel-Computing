#ifndef __IO__
#define __IO__

#include <stdio.h>


void get_input_size(int *input_size);
void load_input(char **input_buffer, int input_size);
void load_ecosystem_specs(
	char* input_buffer,
	int *GEN_PROC_PREY,
	int *GEN_PROC_PREDATOR,
	int *GEN_PREDATOR_FOOD,
	int *N_GEN, 
	int *L, int *C, int *N
);

#endif
