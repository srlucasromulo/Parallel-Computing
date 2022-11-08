#include "io.h"
#include "predator_prey.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void get_input_size(int *input_size){

	FILE *fileptr = popen("wc -c < ./input", "r");

	fscanf(fileptr, "%d", input_size);

	pclose(fileptr);
}

void load_input(char *input_buffer, int input_size){

	FILE *fileptr = fopen("./input", "r");

	fread(input_buffer, sizeof(char), input_size, fileptr);

	fclose(fileptr);
}

void get_next_word(char *input_buffer, int *p, char *string){

	char c;
	strcpy(string, "\0");

	while (c = input_buffer[*p], c != ' ' && c != '\n' && c != '\0') {
		strncat(string, &c, 1);
		(*p)++;
	}
	(*p)++;
}

void load_ecosystem_specs(char* input_buffer, int *specs){

	int p = 0;

	for (int i = 0; i < 7; i++){

		char string[5];

		get_next_word(input_buffer, &p, string);

		specs[i] = atoi(string);
	}
}

void load_ecosystem_disposition(
	char *input_buffer,
	int* specs,
	int *board,
	subject_t *subjects
){
	char c;
	int p = 0;

	// first line = specs
	while (c = input_buffer[p], c != '\n') p++;
	p++;

	for (int i = 0; i < specs[N]; i++){

		char string_label[8];
		char string_position[5];
		int x, y;

		get_next_word(input_buffer, &p, string_label);

		get_next_word(input_buffer, &p, string_position);
		x = atoi(string_position);

		get_next_word(input_buffer, &p, string_position);
		y = atoi(string_position);

		int id = EMPTY_FIELD;
		if (!strcmp(string_label, "ROCHA"))
			id = STONE_FIELD;

		if (!strcmp(string_label, "COELHO") || !strcmp(string_label, "RAPOSA")){

			char type = 
				!strcmp(string_label, "COELHO") ? PREY : PREDATOR;

			id = add_subject_to_list(
				subjects, type, 
				specs[GEN_PROC_PREY], 
				specs[GEN_PROC_PREDATOR], 
				specs[GEN_PREDATOR_FOOD]
			);
		}

		add_object_to_board(board, specs[R], specs[C], id, x, y);

		printf("label %d: %s, %d, %d\n", i, string_label, x, y);
	}
}
