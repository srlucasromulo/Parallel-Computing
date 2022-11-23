#ifndef __PREDATOR_PREY__
#define __PREDATOR_PREY__


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


#define PREDATOR 'R'	// types for subject list
#define PREY 'C'
#define OBSTACLE '*'
#define NONE '-'

#define PREDATOR_ALIAS "RAPOSA"
#define PREY_ALIAS "COELHO"
#define OBSTACLE_ALIAS "ROCHA"


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
	int x, y;
	union {
		predator_t predator;
		prey_t prey;
	};
} subject_t;


/* _____LIST_____ */
void empty_subjects_list(subject_t *list, int size);
void print_subjects_list(const subject_t *list, int N);
void copy_subjects_list(subject_t *dest, const subject_t *src, int N);
void add_subject_to_list(subject_t *list, subject_t item);
subject_t new_obstacle(int x, int y);
subject_t new_prey(int x, int y, int GEN_PROC);
subject_t new_predator(int x, int y, int GEN_PROC, int GEN_FOOD);
char subject_in_position(const subject_t *list, int N, int x, int y);

/* _____MOVEMENT_____ */
void move_preys(
	const subject_t *subjects, subject_t *next_subjects,
	int g, int R, int C, int *N,
	int GEN_PREY
);
void move_predators(
	const subject_t *subjects, subject_t *next_subjects,
	int g, int R, int C, int *N,
	int GEN_PREDATOR, int GEN_FOOD
);
void solve_conflicts(
	subject_t *subjects,
	int *N
);


#endif
