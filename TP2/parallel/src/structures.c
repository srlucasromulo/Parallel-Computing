#include "predator_prey.h"
#include <stdio.h>


/* _____BOARD_____ */
void empty_board(int *board, int R, int C){
	clock_t time = clock();

	#pragma omp parallel for
	for (int i = 0; i < R * C; i++){
		printf("thread %d\n", omp_get_thread_num());
		board[i] = EMPTY_FIELD;
	}

	time = clock() - time;
	printf("%s;%lf\n", __FUNCTION__, (double)time/CLOCKS_PER_SEC);
}

void print_board(int *board, int R, int C){

	printf("\nCURRENT BOARD:\n");
	for (int i = 0; i < R; i++){
		for (int j = 0; j < R; j++)
			printf("%d\t", board[i*C+j]);
		printf("\n");
	}
}

void printf_board(int *board, subject_t *list, int g, int R, int C){

	printf("Gen %d\n", g);
	for (int i = 0; i < C+2; i++)
		printf("-");
	printf("\n");
	for (int i = 0; i < R; i++){
		printf("|");
		for (int j = 0; j < C; j++){
			if (board[i*C+j] == STONE_FIELD) printf("*");
			else if (board[i*C+j] == EMPTY_FIELD) printf("-");
			else printf("%c", list[board[i*C+j]].type);
		}
		printf("|\n");
	}
	for (int i = 0; i < C+2; i++)
		printf("-");
	printf("\n");
}

void copy_board(int *dest, int *src, int size){
	clock_t time = clock();

	for (int i = 0; i < size; i++)
		dest[i] = src[i];

	time = clock() - time;
	printf("%s;%lf\n", __FUNCTION__, (double)time/CLOCKS_PER_SEC);
}

void add_object_to_board(
	int *board, int R, int C,
	int id, int x, int y
){
	board[x*C+y] = id; 
}


/* _____LIST_____ */
void empty_subjects_list(subject_t *list, int R, int C){
	clock_t time = clock();

	for (int i = 0; i < R * C; i++)
		list[i].type = '-';

	time = clock() - time;
	printf("%s;%lf\n", __FUNCTION__, (double)time/CLOCKS_PER_SEC);
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

int new_subject_position(subject_t *list);
int add_subject_to_list(
	subject_t *list, char type,
	int GEN_PROC_PREY,
	int GEN_PROC_PREDATOR,
	int GEN_PREDATOR_FOOD
){

	int p = new_subject_position(list);

	list[p].type = type;

	if (type == PREDATOR) {
		list[p].predator.gen_proc = GEN_PROC_PREDATOR;
		list[p].predator.gen_food = GEN_PREDATOR_FOOD;
	}
	else
		list[p].prey.gen_proc = GEN_PROC_PREY;

	return p;
}

int new_subject_position(subject_t *list){
	clock_t time = clock();

	int id;
	for (id = 0; list[id].type != NONE; id++);

	time = clock() - time;
	printf("%s;%lf\n", __FUNCTION__, (double)time/CLOCKS_PER_SEC);
	
	return id;
}


int new_prey(subject_t *list, int R, int C, int GEN_PROC) {
	
	int id = new_subject_position(list);
	list[id].type = PREY;
	list[id].prey.gen_proc = GEN_PROC;
	return id;
}

int new_predator(subject_t *list, int R, int C, int GEN_PROC, int GEN_FOOD){

	int id = new_subject_position(list);
	list[id].type = PREDATOR;
	list[id].predator.gen_proc = GEN_PROC;
	list[id].predator.gen_food = GEN_FOOD;
	return id;
}
