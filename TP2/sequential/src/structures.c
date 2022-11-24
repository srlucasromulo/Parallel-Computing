#include "predator_prey.h"
#include <stdio.h>


/* _____BOARD_____ */
void print_board(const subject_t *list, int R, int C){

	char board[R][C];
	for (int i = 0; i < R; i++)
		for (int j = 0; j < C; j++)
			board[i][j] = NONE;

	for (int i = 0; i < R*C; i++)
		if (list[i].type != NONE)
			board[list[i].x][list[i].y] = list[i].type;

	for (int i = 0; i < C+2; i++) printf("-");
	printf("\n");

	for (int i = 0; i < R; i++){
		printf("|");
		for (int j = 0; j < C; j++){
			printf("%c", board[i][j]);
		}
		printf("|\n");
	}

	for (int i = 0; i < C+2; i++) printf("-");
	printf("\n");
}


/* _____LIST_____ */
void empty_subjects_list(subject_t *list, int size){

	for (int i = 0; i < size; i++)
		list[i].type = '-';
}

void print_subjects_list(const subject_t *list, int size){

	for (int i = 0; i < size; i++)
		if (list[i].type != NONE){
			if (list[i].type == PREDATOR)
				printf("%s ", PREDATOR_ALIAS);			
			if (list[i].type == PREY)
				printf("%s ", PREY_ALIAS);	
			if (list[i].type == OBSTACLE)
				printf("%s ", OBSTACLE_ALIAS);
			printf("%d %d\n", list[i].x, list[i].y);
		}
}

void copy_subjects_list(subject_t *dest, const subject_t *src, int size){

	for (int i = 0; i < size; i++)
		dest[i] = src[i];
}

void new_subjects_list(subject_t *dest, const subject_t *src, int size){

	empty_subjects_list(dest, size);
	for (int i = 0; i < size; i++)
		if (src[i].type == OBSTACLE)
			add_subject_to_list(dest, src[i]);
}

int empty_list_position(subject_t *list);
void add_subject_to_list(subject_t *list, subject_t item){

	int p = empty_list_position(list);
	list[p] = item;
}

int empty_list_position(subject_t *list){

	int id;
	for (id = 0; list[id].type != NONE; id++);

	return id;
}

subject_t new_obstacle(int x, int y) {
	
	subject_t new;

	new.type = OBSTACLE;
	new.x = x;
	new.y = y;

	return new;
}

subject_t new_prey(int x, int y, int GEN_PROC) {
	
	subject_t new;

	new.type = PREY;
	new.x = x;
	new.y = y;
	new.prey.gen_proc = GEN_PROC;

	return new;
}

subject_t new_predator(int x, int y, int GEN_PROC, int GEN_FOOD){

	subject_t new;

	new.type = PREDATOR;
	new.x = x;
	new.y = y;
	new.predator.gen_proc = GEN_PROC;
	new.predator.gen_food = GEN_FOOD;

	return new;
}

char subject_in_position(const subject_t *list, int size, int x, int y) {

	for (int i = 0; i < size; i++)
		if (list[i].type != NONE)
			if (list[i].x == x && list[i].y == y)
				return list[i].type;
	return NONE;
}
