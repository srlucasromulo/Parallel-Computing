#ifndef __IO__
#define __IO__


#include "predator_prey.h"


void get_input_size(int *input_size);
void load_input(char *input_buffer, int input_size);
void load_ecosystem_specs(char *input_buffer, int *specs);
void load_ecosystem_disposition(
	char *input_buffer,
	int *specs,
	int *current_board,
	subject_t *subjects
);
void save_output(
	int *current_board, 
	subject_t *subjects,
	int *specs
);


#endif
