#include "predator_prey.h"
#include <stdio.h>


/* _____LIST_____ */
void empty_subjects_list(subject_t *list, int size){
	clock_t time = clock();

	for (int i = 0; i < size; i++)
		list[i].type = '-';

	time = clock() - time;
	printf("%s;%lf\n", __FUNCTION__, (double)time/CLOCKS_PER_SEC);
}

void print_subjects_list(const subject_t *list, int N){

	int count_N = 0;

	for (int i = 0; count_N < N; i++){
		if (list[i].type != NONE){
			if (list[i].type == PREDATOR)
				printf("%s ", PREDATOR_ALIAS);			
			if (list[i].type == PREY)
				printf("%s ", PREY_ALIAS);	
			if (list[i].type == OBSTACLE)
				printf("%s ", OBSTACLE_ALIAS);
		printf("%d %d\n", list[i].x, list[i].y);
		count_N++;
		}
	}
}

void copy_subjects_list(subject_t *dest, const subject_t *src, int size){
	clock_t time = clock();

	for (int i = 0; i < size; i++)
		dest[i] = src[i];

	time = clock() - time;
	printf("%s;%lf\n", __FUNCTION__, (double)time/CLOCKS_PER_SEC);
}

int open_list_position(subject_t *list);
void add_subject_to_list(subject_t *list, subject_t item){

	int p = open_list_position(list);
	list[p] = item;
}

int open_list_position(subject_t *list){
	clock_t time = clock();

	int id;
	for (id = 0; list[id].type != NONE; id++);

	time = clock() - time;
	printf("%s;%lf\n", __FUNCTION__, (double)time/CLOCKS_PER_SEC);
	
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

char subject_in_position(const subject_t *list, int N, int x, int y) {

	int count_N = 0;
	for (int i = 0; count_N < N; i++){
		if (list[i].type != NONE){
			if (list[i].x == x && list[i].y == y)
				return list[i].type;
			else
				count_N++;
		}
	}
	return NONE;
}
