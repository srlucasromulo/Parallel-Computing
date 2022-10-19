#include "io.h"


int get_input_size(){

	FILE* fp = popen("wc -l < ./input", "r");

	int size;
	fscanf(fp, "%d", &size);

	fclose(fp);

	return size;
}

void read_file(int* values, int size){

	FILE* fp = fopen("./input", "r");

	for (int i = 0; i < size; i++)
		fscanf(fp, "%d\n", &values[i]);

	fclose(fp);
}

void write_file(int* values, int size){

	FILE* fp = fopen("./output", "w");

	for (int i = 0; i < size; i++)
		fprintf(fp, "%d\n", values[i]);

	fclose(fp);
}

void write_cpu_time(float cpu){

	FILE* fp = fopen("./time", "w");

	fprintf(fp, "cpu;%f\n", cpu);

	fclose(fp);
}

void write_p_time(float time, int rank){

	FILE* fp = fopen("./time", "a");

	fprintf(fp, "%d;%f\n", rank, time);

	fclose(fp);
}
