#ifndef __PREDATOR_PREY__
#define __PREDATOR_PREY__


#define PREDATOR 'X'	// types for subject list
#define PREY 'O'
#define NONE '-'

#define EMPTY_FIELD -1
#define STONE_FIELD -2

enum spec_t {
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


void empty_board(int *board, int R, int C);
void print_board(int *board, int R, int C);
void empty_subjects_list(subject_t *list, int R, int C);
void print_subjects_list(subject_t *list, int R, int C);

int add_subject_to_list(
	subject_t *list, char type,
	int GEN_PROC_PREY,
	int GEN_PROC_PREDATOR,
	int GEN_PREDATOR_FOOD
);

void add_object_to_board(
	int *board, int R, int C,
	int id, int x, int y
);


#endif
