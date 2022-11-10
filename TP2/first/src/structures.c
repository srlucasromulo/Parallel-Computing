#include "predator_prey.h"
#include <stdio.h>


/* _____BOARD_____ */
void empty_board(int *board, int R, int C){

	for (int i = 0; i < R * C; i++)
		board[i] = EMPTY_FIELD;
}

void print_board(int *board, int R, int C){

	printf("\nCURRENT BOARD:\n");
	for (int i = 0; i < R; i++){
		for (int j = 0; j < R; j++)
			printf("%d\t", board[i*C+j]);
		printf("\n");
	}
}

void copy_board(int *dest, int *src, int size){

	for (int i = 0; i < size; i++)
		dest[i] = src[i];
}

void add_object_to_board(
	int *board, int R, int C,
	int id, int x, int y
){
	board[x*C+y] = id; 
}


/* _____LIST_____ */
void empty_subjects_list(subject_t *list, int R, int C){

	for (int i = 0; i < R * C; i++)
		list[i].type = '-';
}

void print_subjects_list(subject_t *list, int R, int C){

	for (int i = 0; i < R * C; i++){
		if (list[i].type == PREDATOR || list[i].type == PREY)
			printf("%d- ", i);
		if (list[i].type == PREY)
			printf("%c, %d\n", list[i].type, list[i].prey.gen_proc);
		if (list[i].type == PREDATOR)
			printf("%c, %d, %d\n", list[i].type, list[i].predator.gen_proc, list[i].predator.gen_food);
	}
}

int add_subject_to_list(
	subject_t *list, char type,
	int GEN_PROC_PREY,
	int GEN_PROC_PREDATOR,
	int GEN_PREDATOR_FOOD
){
	int p = 0;

	while (list[p].type != NONE) p++;

	list[p].type = type;

	if (type == PREDATOR) {
		list[p].predator.gen_proc = GEN_PROC_PREDATOR;
		list[p].predator.gen_food = GEN_PREDATOR_FOOD;
	}
	else
		list[p].prey.gen_proc = GEN_PROC_PREY;

	return p;
}
