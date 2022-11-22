#ifndef __PREDATOR_PREY__
#define __PREDATOR_PREY__


#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define PREDATOR 'R'	// types for subject list
#define PREY 'C'
#define NONE '-'

#define EMPTY_FIELD -1
#define STONE_FIELD -2

enum specs_enum {
	GEN_PROC_PREY, GEN_PROC_PREDATOR, GEN_PREDATOR_FOOD,
	N_GEN, R, C, N = 6
};


typedef struct predator_t {
	int gen_proc;
	int gen_food;
} predator_t;

typedef struct prey_t {
	int gen_proc;
} prey_t;

typedef struct subject_t {
	char type;
	union {
		predator_t predator;
		prey_t prey;
	};
} subject_t;


/* _____BOARD_____ */
void empty_board(int *board, int R, int C);
void print_board(int *board, int R, int C);
void printf_board(int *board, subject_t *list, int g, int R, int C);
void copy_board(int *dest, int *src, int size);
void add_object_to_board(
	int *board, int R, int C,
	int id, int x, int y
);

/* _____LIST_____ */
void empty_subjects_list(subject_t *list, int R, int C);
void print_subjects_list(subject_t *list, int R, int C);
int add_subject_to_list(
	subject_t *list, char type,
	int GEN_PROC_PREY,
	int GEN_PROC_PREDATOR,
	int GEN_PREDATOR_FOOD
);
int new_prey(subject_t *list, int R, int C, int GEN_PROC);
int new_predator(subject_t *list, int R, int C, int GEN_PROC, int GEN_FOOD);

/* _____MOVEMENT_____ */
void move_preys(
	int *current_board,
	int *next_board, 
	subject_t *subjects,
	int g, int R, int C, int *N,
	int GEN_PREY
);
void move_predators(
	int *current_board,
	int *next_board, 
	subject_t *subjects,
	int g, int R, int C, int *N,
	int GEN_PREDATOR, int GEN_FOOD
);


#endif
