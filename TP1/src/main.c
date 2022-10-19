#include <stdlib.h>
#include <time.h>
#include "io.h"
#include "compute.h"
#include "mpi.h"


#define TRUE 1
#define FALSE 0
#define MASTER 0


typedef struct time_spent_t {
    time_t begin;
    time_t end;
} time_spent_t;


int main(int argc, char* argv[])
{
    int rank, mpi_size;     // MPI
    
    int file_size;          // file lines
    int* file_buffer;       // entire file (input n output)
    int* buffer;            // process buffer
    int N;                  // size of process buffer

    time_spent_t* time_cpu; // cpu usage time
    time_spent_t* time_p = malloc (sizeof(time_spent_t));   // process time
    double diff;            // diff = time->end - time->begin

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    if (rank == MASTER) {
        // get file size (in lines)
        file_size = get_input_size();

        // calc process buffer size
        N = file_size / mpi_size;

        // read input file
        file_buffer = (int*) malloc (file_size * sizeof(int));
        read_file(file_buffer, file_size);

        // init cpu timer
        time_cpu  = malloc (sizeof(time_spent_t));
        time_cpu->begin  = clock();
    }

    // broadcast process buffer size
    MPI_Bcast(&N, 1, MPI_INT, MASTER, MPI_COMM_WORLD);

    // init buffer
    buffer = (int*) malloc (N * sizeof(int));

    // scatter input values 
    MPI_Scatter(file_buffer, N, MPI_INT, buffer, N, MPI_INT, MASTER, MPI_COMM_WORLD);

    // compute process buffered values and time spent
    time_p->begin = clock();
    compute_array(buffer, N, rank);
    time_p->end = clock();

    // gather output values
    MPI_Gather(buffer, N, MPI_INT, file_buffer, N, MPI_INT, MASTER, MPI_COMM_WORLD);

    if (rank == MASTER){
        // end cpu time and calc diff
        time_cpu->end  = clock();
        diff = (double)(time_cpu->end - time_cpu->begin);

        // write cpu time to file
        write_cpu_time(diff / CLOCKS_PER_SEC);

        // write output values to file
        write_file(file_buffer, file_size);
    }

    // waits until all process gets here
    MPI_Barrier(MPI_COMM_WORLD);

    // calc process time spent and writes into file
    diff = (double)(time_p->end - time_p->begin);
    write_p_time(diff / CLOCKS_PER_SEC, rank);

    MPI_Finalize();
}
