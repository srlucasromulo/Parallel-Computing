#ifndef __IO__
#define __IO__


#include <stdio.h>


int get_input_size();
void read_file(int* values, int size);
void write_file(int* values, int size);

void write_cpu_time(float cpu);
void write_p_time(float time, int rank);


#endif
