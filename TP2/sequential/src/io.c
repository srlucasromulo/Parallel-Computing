#include "io.h"
#include "predator_prey.h"
#include <stdio.h>
#include <string.h>


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

		subject_t new;
		if (!strcmp(string_label, OBSTACLE_ALIAS))
			new = new_obstacle(x, y);
		if (!strcmp(string_label, PREY_ALIAS))
			new = new_prey(x, y, specs[GEN_PROC_PREY]);
		if (!strcmp(string_label, PREDATOR_ALIAS))
			new = new_predator(x, y, specs[GEN_PROC_PREDATOR], specs[GEN_PREDATOR_FOOD]);	

		add_subject_to_list(subjects, new);
	}
}

void save_output(
	subject_t *subjects,
	int *specs
){

	char buffer[1024] = "\0";

	for (int i = 0; i < 7; i++){
		char value[5];
		sprintf(value, "%d ", specs[i]);
		strcat(buffer, value);
	}
	strcat(buffer, "\n");

	int n_ = 0;
	for (int i = 0; n_ < specs[N]; i++){
		if (subjects[i].type != NONE){
			if (subjects[i].type == OBSTACLE)
				strcat(buffer, "ROCHA ");
			if (subjects[i].type == PREY)
				strcat(buffer, "COELHO ");
			if (subjects[i].type == PREDATOR)
				strcat(buffer, "RAPOSA ");
			char position[11] = "\0";
			sprintf(position, "%d %d\n", subjects[i].x, subjects[i].y);
			strcat(buffer, position);
			n_++;		
		}
	}

	FILE *fileptr = fopen("output", "w");
	fwrite(buffer, sizeof(char), sizeof(buffer), fileptr);
	fclose(fileptr);
}
